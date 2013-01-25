#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include <GLWT/glwt.h>
//#include <GLXW/glxw.h>
#include <GL/gl.h>

struct painter
{
    pthread_mutex_t lock;
    pthread_cond_t state_changed;
    int stopped, paused, visible, exposed;
};

static void painter_init(struct painter *painter)
{
    pthread_mutex_init(&painter->lock, NULL);
    pthread_cond_init(&painter->state_changed, NULL);

    painter->stopped = 0;
    painter->paused = 1;
    painter->visible = 0;
    painter->exposed = 0;
}

static void painter_destroy(struct painter *painter)
{
    pthread_mutex_destroy(&painter->lock);
    pthread_cond_destroy(&painter->state_changed);
}

static int painter_wait(struct painter *painter)
{
    pthread_mutex_lock(&painter->lock);

    while(!painter->stopped &&
        (!painter->visible ||
            (painter->paused && !painter->exposed)))
    {
        pthread_cond_wait(&painter->state_changed, &painter->lock);
    }

    int stopped = painter->stopped;
    painter->exposed = 0;

    pthread_mutex_unlock(&painter->lock);

    return !stopped;
}

static int painter_pause(struct painter *painter, int paused)
{
    pthread_mutex_lock(&painter->lock);

    if(!paused && !painter->stopped && painter->paused)
        pthread_cond_signal(&painter->state_changed);

    painter->paused = paused;

    pthread_mutex_unlock(&painter->lock);

    return 0;
}

static int painter_stop(struct painter *painter)
{
    pthread_mutex_lock(&painter->lock);

    painter->stopped = 1;
    pthread_cond_signal(&painter->state_changed);

    pthread_mutex_unlock(&painter->lock);

    return 0;
}

static int painter_expose(struct painter *painter)
{
    pthread_mutex_lock(&painter->lock);

    if(!painter->stopped && painter->paused && !painter->exposed)
        pthread_cond_signal(&painter->state_changed);

    painter->exposed = 1;

    pthread_mutex_unlock(&painter->lock);

    return 0;
}

static int painter_visible(struct painter *painter, int visible)
{
    pthread_mutex_lock(&painter->lock);

    if(visible && !painter->stopped && !painter->visible && (!painter->paused || painter->exposed))
        pthread_cond_signal(&painter->state_changed);
    painter->visible = visible;

    pthread_mutex_unlock(&painter->lock);

    return 0;
}

static int painter_stopped(struct painter *painter)
{
    pthread_mutex_lock(&painter->lock);
    int stopped = painter->stopped;
    pthread_mutex_unlock(&painter->lock);

    return stopped;
}

struct test_app;

struct test_window
{
    GLWTWindow *window;
    pthread_t thread;

    struct painter painter;

    struct test_app *app;
    struct test_window *next_window, *prev_window;
};

struct test_app
{
    GLWTWindow *master_window;
    pthread_t background_thread;

    struct test_window *window_list;
};

static void *background_main(void *arg)
{
    struct test_app *app = (struct test_app *)arg;

    glwtMakeCurrent(app->master_window);

    printf("background thread\n");
    printf("GL_VERSION: %s\n", glGetString(GL_VERSION));
    printf("GL_VENDOR: %s\n", glGetString(GL_VENDOR));

    glwtMakeCurrent(NULL);

    return app;
}

static void* painter_main(void *arg)
{
    struct test_window *tw = (struct test_window*)arg;
    GLWTWindow *win = tw->window;

    glwtMakeCurrent(win);
    glwtSwapInterval(win, 1);

    while(painter_wait(&tw->painter) == 1)
    {
        int width, height;
        glwtWindowGetSize(win, &width, &height);

        glClear(GL_COLOR_BUFFER_BIT);
        printf("paint %p\n", arg);

        glwtSwapBuffers(win);

        printf("painted\n");
    }

    glwtMakeCurrent(NULL);
    return tw;
}

static struct test_window *test_window_create(struct test_app *app, const char *title, int width, int height);

static void win_event_callback(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata)
{
    (void)window;
    struct test_window *tw = (struct test_window *)userdata;

    switch(event->type)
    {
        case GLWT_WINDOW_CLOSE:
            painter_stop(&tw->painter);
            break;

        case GLWT_WINDOW_EXPOSE:
            painter_expose(&tw->painter);
            break;

        case GLWT_WINDOW_SHOW:
        case GLWT_WINDOW_HIDE:
            painter_visible(&tw->painter, event->type == GLWT_WINDOW_SHOW);
            break;

        case GLWT_WINDOW_KEY_DOWN:
            if(event->key.keysym == GLWT_KEY_ESCAPE)
                painter_stop(&tw->painter);
            else if(event->key.keysym == GLWT_KEY_N)
            {
                struct test_window *nw = test_window_create(tw->app, "", 640, 480);
                glwtWindowShow(nw->window, 1);
                painter_pause(&nw->painter, 0);
            }
            break;

        default:
            break;
    }
}

static struct test_window *test_window_create(struct test_app *app, const char *title, int width, int height)
{
    struct test_window *tw = calloc(1, sizeof(struct test_window));
    if(!tw) return NULL;

    tw->window = glwtWindowCreate(title, width, height, app->master_window, win_event_callback, (void*)tw);
    if(!tw->window)
    {
        free(tw);
        return NULL;
    }

    painter_init(&tw->painter);

    // This does not need a mutex as long as this function is only called from the main thread
    tw->app = app;
    tw->next_window = app->window_list;
    if(tw->next_window)
        tw->next_window->prev_window = tw;
    tw->prev_window = NULL;
    app->window_list = tw;

    // launch thread now
    if(pthread_create(&tw->thread, NULL, painter_main, (void*)tw) != 0)
    {
    }

    return tw;
}

static int test_window_destroy(struct test_window *tw)
{
    if(tw->prev_window)
        tw->prev_window->next_window = tw->next_window;
    else
        tw->app->window_list = tw->next_window;

    if(tw->next_window)
        tw->next_window->prev_window = tw->prev_window;

    int err = 0;
    void *ret = 0;
    if(pthread_join(tw->thread, &ret) != 0)
        err = -1;

    painter_destroy(&tw->painter);

    glwtWindowDestroy(tw->window);

    free(tw);

    return err == 0 && ret == tw ? 0 : -1;
}

static struct test_app *test_app_create()
{
    struct test_app *app = calloc(1, sizeof(struct test_app));
    if(!app) return NULL;

    app->master_window = glwtWindowCreate("", 640, 480, NULL, NULL, NULL);

    if(pthread_create(&app->background_thread, NULL, background_main, app) != 0)
    {
    }

    return app;
}

static int test_app_destroy(struct test_app *app)
{
    void* ret = 0;

    if(pthread_join(app->background_thread, &ret) != 0)
        ret = 0;

    glwtWindowDestroy(app->master_window);

    free(app);

    return ret == app ? 0 : -1;
}

static int test_app_run(struct test_app *app)
{
    while(app->window_list)
    {
        glwtEventHandle(1);

        struct test_window *tw = 0, *next = app->window_list;
        while(next)
        {
            tw = next;
            next = tw->next_window;

            if(painter_stopped(&tw->painter))
            {
                if(test_window_destroy(tw) != 0)
                    return -1;
            }
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    if(glwtInit(NULL, NULL, NULL) != 0)
        return -1;

    struct test_app *app = test_app_create();

    for(int i = 0; i < 2; ++i)
    {
        struct test_window *tw = test_window_create(app, "", 640, 480);
        glwtWindowShow(tw->window, 1);
        painter_pause(&tw->painter, 0);
    }

    test_app_run(app);

    test_app_destroy(app);

    glwtQuit();

    return 0;
}
