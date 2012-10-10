#include <stdlib.h>

#include <glwt_internal.h>

GLWTWindow *glwtWindowCreate(
    const char *title,
    int width, int height,
    const GLWTWindowCallbacks *win_callbacks,
    GLWTWindow *share)
{
    GLWTWindow *win = calloc(1, sizeof(GLWTWindow));
    if(!win)
        return 0;

    (void)title; // TODO: set window title
    if(win_callbacks)
        win->win_callbacks = *win_callbacks;

    XSetWindowAttributes attrib;
    attrib.colormap = glwt.x11.colormap;
    attrib.event_mask = StructureNotifyMask | ExposureMask | KeyPressMask;
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
}
