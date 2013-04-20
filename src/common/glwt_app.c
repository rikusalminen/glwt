#include <glwt_internal.h>

// TODO: this must be synchronized to make glwtAppTerminate thread safe
static int terminated = 0;

int glwtAppMain(int argc, char *argv[])
{
    GLWTWindow *window = glwtAppInit(argc, argv);
    if(!window)
        return 1;

    // TODO: app start event

    glwtWindowShow(window, 1);

    if(window->win_callback)
    {
        GLWTWindowEvent event;
        event.window = window;
        event.type = GLWT_WINDOW_SURFACE_CREATE;
        window->win_callback(window, &event, window->userdata);
    }

    while(!terminated && !glwtWindowClosed(window))
        if(glwtEventHandle(1) != 0)
            return 1;

    if(window->win_callback)
    {
        GLWTWindowEvent event;
        event.window = window;
        event.type = GLWT_WINDOW_SURFACE_DESTROY;
        window->win_callback(window, &event, window->userdata);
    }

    // TODO: app stop event

    glwtWindowShow(window, 0);
    glwtWindowDestroy(window);

    glwtQuit();

    return 0;
}

void glwtAppTerminate()
{
    terminated = 1;
}

