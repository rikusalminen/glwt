#include <glwt_internal.h>

int glwtWindowCreateWGL(GLWTWindow *win, GLWTWindow *share)
{
    int context_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, glwt.api_version_major != 0 ? glwt.api_version_major : 1,
        WGL_CONTEXT_MINOR_VERSION_ARB, glwt.api_version_minor,
        WGL_CONTEXT_FLAGS_ARB,
            ((glwt.api & GLWT_PROFILE_DEBUG) ? WGL_CONTEXT_DEBUG_BIT_ARB : 0) |
            ((glwt.api & GLWT_PROFILE_ROBUSTNESS) ? WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB : 0),
        WGL_CONTEXT_PROFILE_MASK_ARB,
            ((glwt.api & GLWT_API_MASK) != GLWT_API_OPENGL_ES ?
                ((glwt.api & GLWT_PROFILE_COMPATIBILITY) ?
                    WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB : WGL_CONTEXT_CORE_PROFILE_BIT_ARB) : 0) |
            ((glwt.api & GLWT_API_MASK) == GLWT_API_OPENGL_ES ? WGL_CONTEXT_ES2_PROFILE_BIT_EXT : 0),
            0, 0,
    };

    win->wgl.context = wglCreateContextAttribsARB(win->win32.hdc, share ? share->wgl.context : 0, context_attribs);
    if(!win->wgl.context)
    {
        glwtWin32Error("wglCreateContextAttribsARB failed");
        goto error;
    }

    return 0;
error:
    glwtWindowDestroyWGL(win);
    return -1;
}

void glwtWindowDestroyWGL(GLWTWindow *win)
{
    if(win->wgl.context)
        wglDeleteContext(win->wgl.context);
}

int glwtMakeCurrent(GLWTWindow *win)
{
    if(!wglMakeContextCurrentARB(
        win ? win->win32.hdc : 0,
        win ? win->win32.hdc : 0,
        win ? win->wgl.context : 0))
    {
        glwtWin32Error("wglMakeContextCurrentARB failed");
        return -1;
    }

    return 0;
}

int glwtSwapBuffers(GLWTWindow *win)
{
    if(!SwapBuffers(win->win32.hdc))
    {
        glwtWin32Error("SwapBuffers failed");
        return -1;
    }

    return 0;
}

int glwtSwapInterval(GLWTWindow *win, int interval)
{
    (void)win;
    if(!wglSwapIntervalEXT(interval))
    {
        glwtWin32Error("wglSwapIntervalEXT failed");
        return -1;
    }

    return 0;
}

int glwtWindowGetSize(GLWTWindow *win, int *width, int *height)
{
    RECT rect;
    if(!GetClientRect(win->win32.hwnd, &rect))
    {
        glwtWin32Error("GetClientRect failed");
        return -1;
    }

    *width = rect.right - rect.left;
    *height = rect.bottom - rect.top;
    return 0;
}
