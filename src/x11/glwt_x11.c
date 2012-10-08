#include <stdlib.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

static int xlib_io_error(Display *display)
{
    (void)display;
    glwtErrorPrintf("Xlib I/O Error");
    exit(EXIT_FAILURE);
    return 0;
}

static int xlib_error(Display *display, XErrorEvent *event)
{
    (void)display;

    int buffer_size = 128;
    char buffer[buffer_size];
    XGetErrorText(display, event->error_code, buffer, buffer_size);

    glwtErrorPrintf("Xlib error code: %d  request: %d  message: %s\n",
        event->error_code,
        event->request_code,
        buffer);
    return 0;
}

int glwtInit(const GLWTConfig *config, const GLWTAppCallbacks *app_callbacks)
{
    if(app_callbacks)
        glwt.app_callbacks = *app_callbacks;

    XInitThreads();

    XSetIOErrorHandler(xlib_io_error);
    XSetErrorHandler(xlib_error);

    if((glwt.x11.display = XOpenDisplay(NULL)) == NULL)
    {
        glwtErrorPrintf("Can't open display");
        return -1;
    }

    glwt.x11.screen_num = DefaultScreen(glwt.x11.display);

#ifdef GLWT_USE_EGL
    if(glwtInitEGL(config) != 0)
#else
    if(glwtInitGLX(config) != 0)
#endif
        goto error;

    glwt.x11.colormap = XCreateColormap(
        glwt.x11.display,
        RootWindow(glwt.x11.display, glwt.x11.screen_num),
        glwt.x11.visual,
        AllocNone);

    return 0;
error:
    XFreeColormap(glwt.x11.display, glwt.x11.colormap);
    XCloseDisplay(glwt.x11.display);
    return -1;
}

void glwtQuit()
{
#ifdef GLWT_USE_EGL
    glwtQuitEGL();
#else
    glwtQuitGLX();
#endif

    XFreeColormap(glwt.x11.display, glwt.x11.colormap);
    XCloseDisplay(glwt.x11.display);
}
