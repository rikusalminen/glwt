#include <stdio.h>

#include <GLWT/glwt.h>

#ifdef GLWT_TEST_GLES
#include <GLXW/glxw_es2.h>
#else
#include <GLXW/glxw.h>
#endif

#ifdef ANDROID
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, __FILE__, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, __FILE__, __VA_ARGS__))
#else
#define LOGI(...) ((void)fprintf(stdout, __VA_ARGS__))
#define LOGW(...) ((void)fprintf(stderr, __VA_ARGS__))
#endif

static void error_callback(const char *msg, void *userdata)
{
    (void)userdata;
#ifdef ANDROID
    LOGW(msg);
#else
    fprintf(stderr, "%s\n", msg);
#endif
}

static void window_callback(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata)
{
    (void)window;
    (void)userdata;

    switch(event->type)
    {
        case GLWT_WINDOW_CLOSE:
            LOGI("Window closed\n");
            break;
        case GLWT_WINDOW_SURFACE_CREATE:
            LOGI("Surface create\n");
            glwtMakeCurrent(window);

            if(glxwInit() != 0)
            {
                LOGW("glxwInit failed\n");
                glwtAppTerminate();
            }

            break;

        case GLWT_WINDOW_SURFACE_DESTROY:
            LOGI("Surface destroy\n");
            glwtMakeCurrent(0);
            break;

        case GLWT_WINDOW_EXPOSE:
            LOGI("Window exposed\n");
            {
                int width, height;
                glwtWindowGetSize(window, &width, &height);
                LOGI("**** window size: %d x %d\n", width, height);

                glClearColor(0.2, 0.4, 0.7, 1.0);
                glClear(GL_COLOR_BUFFER_BIT);

                glwtSwapBuffers(window);
            }
            break;
        case GLWT_WINDOW_RESIZE:
            LOGI("Window resized  width: %d  height: %d\n", event->resize.width, event->resize.height);
            break;
        case GLWT_WINDOW_SHOW:
        case GLWT_WINDOW_HIDE:
            LOGI("Window %s\n", (event->type == GLWT_WINDOW_SHOW) ? "show" : "hide");
            break;
        case GLWT_WINDOW_FOCUS_IN:
        case GLWT_WINDOW_FOCUS_OUT:
            LOGI("Window focus %s\n", (event->type == GLWT_WINDOW_FOCUS_IN) ? "in" : "out");
            break;
        case GLWT_WINDOW_KEY_UP:
        case GLWT_WINDOW_KEY_DOWN:
            LOGI("Key %s  keysym: 0x%x  scancode: %d  mod: %X\n",
                (event->type == GLWT_WINDOW_KEY_DOWN) ? "down" : "up",
                event->key.keysym, event->key.scancode, event->key.mod);
            break;
        case GLWT_WINDOW_BUTTON_UP:
        case GLWT_WINDOW_BUTTON_DOWN:
            LOGI("Button %s  x: %d  y: %d  button: %d  mod: %X\n",
                (event->type == GLWT_WINDOW_BUTTON_DOWN) ? "down" : "up",
                event->button.x, event->button.y, event->button.button, event->button.mod);
            break;
        case GLWT_WINDOW_MOUSE_MOTION:
            LOGI("Motion  x: %d  y: %d  buttons: %X\n",
                event->motion.x, event->motion.y, event->motion.buttons);
            break;
        case GLWT_WINDOW_MOUSE_ENTER:
        case GLWT_WINDOW_MOUSE_LEAVE:
            LOGI("Mouse %s\n", (event->type == GLWT_WINDOW_MOUSE_ENTER) ? "enter" : "leave");
            break;
        default:
            break;
    }
}

GLWTWindow *glwtAppInit(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    GLWTConfig glwt_config = {
        0, 0, 0, 0,
        0, 0,
        0, 0,
        GLWT_API_ANY | GLWT_PROFILE_DEBUG,
        2, 0
    };

    if(glwtInit(&glwt_config, error_callback, NULL) != 0)
        return 0;

    return glwtWindowCreate("", 400, 300, NULL, window_callback, NULL);
}

#ifndef ANDROID
int main(int argc, char *argv[]) { return glwtAppMain(argc, argv); }
#endif
