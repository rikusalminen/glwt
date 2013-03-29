#include <glwt_internal.h>

int glwtWindowCreateContextEGL(GLWTWindow *win, GLWTWindow *share)
{
    int context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, glwt.api_version_major != 0 ? glwt.api_version_major : 1,
        EGL_NONE
    };

    if(!eglBindAPI((glwt.api & GLWT_API_MASK) == GLWT_API_OPENGL ?
        EGL_OPENGL_API : EGL_OPENGL_ES_API))
    {
        glwtErrorPrintf("eglBindAPI failed");
        return -1;
    }

    win->egl.context = eglCreateContext(
        glwt.egl.display,
        glwt.egl.config,
        share ? share->egl.context : EGL_NO_CONTEXT,
        context_attribs);
    if(!win->egl.context)
    {
        glwtErrorPrintf("eglCreateContext failed");
        return -1;
    }

    return 0;
}

int glwtWindowCreateSurfaceEGL(GLWTWindow *win, EGLNativeWindowType native_window)
{
    int surface_attribs[] = {
        EGL_NONE
    };

    win->egl.surface = eglCreateWindowSurface(
        glwt.egl.display,
        glwt.egl.config,
        native_window,
        surface_attribs);
    if(!win->egl.surface)
    {
        glwtErrorPrintf("eglCreateWindowSurface failed");
        return -1;
    }

    return 0;
}

int glwtWindowCreateEGL(GLWTWindow *win, GLWTWindow *share, EGLNativeWindowType native_window)
{
    if(glwtWindowCreateContextEGL(win, share) != 0 ||
        glwtWindowCreateSurfaceEGL(win, native_window) != 0)
    {
        glwtWindowDestroyEGL(win);
        return -1;
    }

    return 0;
}

void glwtWindowDestroySurfaceEGL(GLWTWindow *win)
{
    if(win->egl.surface)
        eglDestroySurface(glwt.egl.display, win->egl.surface);
    win->egl.surface = 0;
}

void glwtWindowDestroyEGL(GLWTWindow *win)
{
    if(win->egl.context)
        eglDestroyContext(glwt.egl.display, win->egl.context);
    if(win->egl.surface)
        eglDestroySurface(glwt.egl.display, win->egl.surface);
}

int glwtMakeCurrent(GLWTWindow *win)
{
    if(!eglMakeCurrent(
        glwt.egl.display,
        win ? win->egl.surface : EGL_NO_SURFACE,
        win ? win->egl.surface : EGL_NO_SURFACE,
        win ? win->egl.context : EGL_NO_CONTEXT))

    {
        glwtErrorPrintf("eglMakeCurrent failed: %x\n", eglGetError());
        return -1;
    }

    return 0;
}

int glwtSwapBuffers(GLWTWindow *win)
{
    if(!eglSwapBuffers(glwt.egl.display, win->egl.surface))
    {
        glwtErrorPrintf("eglMakeCurrent failed: %x\n", eglGetError());
        return -1;
    }

    return 0;
}

int glwtSwapInterval(GLWTWindow *win, int interval)
{
    (void)win;

    if(!eglSwapInterval(glwt.egl.display, interval))
    {
        glwtErrorPrintf("eglSwapInterval failed: %x\n", eglGetError());
        return -1;
    }

    return 0;
}

int glwtWindowGetSize(GLWTWindow *win, int *width, int *height)
{
    if(!eglQuerySurface(glwt.egl.display, win->egl.surface, EGL_WIDTH, width) ||
        !eglQuerySurface(glwt.egl.display, win->egl.surface, EGL_HEIGHT, height))
    {
        glwtErrorPrintf("eglQuerySurface failed: %x\n", eglGetError());
        return -1;
    }

    return 0;
}
