#include <stdlib.h>

#include <glwt_internal.h>

GLWTWindow *glwtWindowCreate(
    const char *title,
    int width, int height,
    GLWTWindow *share,
    void (*win_callback)(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata),
    void *userdata)
{
    GLWTWindow *win = calloc(1, sizeof(GLWTWindow));
    if(!win)
        return 0;

    (void)title; // TODO: set window title

    win->win_callback = win_callback;
    win->userdata = userdata;

    XSetWindowAttributes attrib;
    attrib.colormap = glwt.x11.colormap;
    attrib.event_mask = 0
        | StructureNotifyMask
        | PointerMotionMask
        | ButtonPressMask
        | ButtonReleaseMask
        | KeyPressMask
        | KeyReleaseMask
        | EnterWindowMask
        | LeaveWindowMask
        | FocusChangeMask
        | ExposureMask;
    unsigned long attrib_mask = CWColormap | CWEventMask;

    win->x11.window = XCreateWindow(
        glwt.x11.display,
        RootWindow(glwt.x11.display, glwt.x11.screen_num),
        0, 0, width, height,
        0,
        glwt.x11.depth,
        InputOutput,
        glwt.x11.visual,
        attrib_mask,
        &attrib);

    Atom protocols[] = {
        glwt.x11.atoms.WM_DELETE_WINDOW,
        glwt.x11.atoms._NET_WM_PING,
    };
    int num_protocols = sizeof(protocols)/sizeof(*protocols);
    if(XSetWMProtocols(glwt.x11.display, win->x11.window, protocols, num_protocols) == 0)
    {
        glwtErrorPrintf("XSetWMProtocols failed");
        goto error;
    }

#ifdef GLWT_USE_EGL
    if(glwtWindowCreateEGL(win, share) != 0)
#else
    if(glwtWindowCreateGLX(win, share) != 0)
#endif
        goto error;

    if(XSaveContext(glwt.x11.display, win->x11.window, glwt.x11.xcontext, (XPointer)win) != 0)
    {
        glwtErrorPrintf("XSaveContext failed");
        goto error;
    }

    return win;
error:
    glwtWindowDestroy(win);
    return 0;
}

void glwtWindowDestroy(GLWTWindow *win)
{
    if(!win)
        return;

    if(XDeleteContext(glwt.x11.display, win->x11.window, glwt.x11.xcontext) != 0)
        glwtErrorPrintf("XDeleteContext failed");

#ifdef GLWT_USE_EGL
    glwtWindowDestroyEGL(win);
#else
    glwtWindowDestroyGLX(win);
#endif

    if(win->x11.window)
        XDestroyWindow(glwt.x11.display, win->x11.window);

    free(win);
}

void glwtWindowShow(GLWTWindow *win, int show)
{
    if(show)
        XMapRaised(glwt.x11.display, win->x11.window);
    else
        XUnmapWindow(glwt.x11.display, win->x11.window);
    XFlush(glwt.x11.display);
}
