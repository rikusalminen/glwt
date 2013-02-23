#include <glwt_internal.h>

int glwtWindowCreateEGL(GLWTWindow *win, GLWTWindow *share)
{
    if(!eglBindAPI((glwt.api & GLWT_API_MASK) == GLWT_API_OPENGL ?
        EGL_OPENGL_API : EGL_OPENGL_ES_API))
    {
        glwtErrorPrintf("eglBindAPI failed");
        goto error;
    }

    int context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, glwt.api_version_major != 0 ? glwt.api_version_major : 1,
        EGL_NONE
    };

    win->egl.context = eglCreateContext(
        glwt.egl.display,
        glwt.egl.config,
        share ? share->egl.context : EGL_NO_CONTEXT,
        context_attribs);
    if(!win->egl.context)
    {
        glwtErrorPrintf("eglCreateContext failed");
        goto error;
    }

    int surface_attribs[] = {
        EGL_NONE
    };

    win->egl.surface = eglCreateWindowSurface(
        glwt.egl.display,
        glwt.egl.config,
#ifdef GLWT_glwt_x11_h
        win->x11.window,
#elif defined(GLWT_glwt_rpi_h)
        &(win->rpi.nativewindow),
#elif defined(ANDROID)
        NULL,
#else
#error EGL: unsupported windowing API
#endif
        surface_attribs);
    if(!win->egl.surface)
    {
        glwtErrorPrintf("eglCreateWindowSurface failed");
        goto error;
    }

    return 0;
error:
    glwtWindowDestroyEGL(win);
    return -1;
}

void glwtWindowDestroyEGL(GLWTWindow *win)
{
    if(win->egl.context)
        eglDestroyContext(glwt.egl.display, win->egl.context);
    if(win->egl.surface)
        eglDestroySurface(glwt.egl.display, win->egl.surface);
}

void glwtMakeCurrent(GLWTWindow *win)
{
    eglMakeCurrent(glwt.egl.display, win->egl.surface, win->egl.surface, win->egl.context);
}

void glwtSwapBuffers(GLWTWindow *win)
{
    eglSwapBuffers(glwt.egl.display, win->egl.surface);
}

void glwtSwapInterval(GLWTWindow *win, int interval)
{
    (void)win;
    eglSwapInterval(glwt.egl.display, interval);
}

void glwtWindowGetSize(GLWTWindow *win, int *width, int *height)
{
    eglQuerySurface(glwt.egl.display, win->egl.surface, EGL_WIDTH, width);
    eglQuerySurface(glwt.egl.display, win->egl.surface, EGL_HEIGHT, height);
}
