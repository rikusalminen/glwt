#include <glwt_internal.h>
#include <stdlib.h>

#include <malloc.h>

GLWTWindow *glwtWindowCreate(
    const char *title,
    int width, int height,
    GLWTWindow *share,
    void *userdata)
{
    PIXELFORMATDESCRIPTOR pfd;
    RECT rect;
    int style;
    int exstyle;
    WCHAR nullterm;
    GLWTWindow *win;

    if(!(win = calloc(1, sizeof(struct GLWTWindow))))
        return 0;

    win->userdata = userdata;

    rect.left = rect.top = 0;
    rect.right = width;
    rect.bottom = height;

    style = WS_OVERLAPPEDWINDOW | WS_THICKFRAME;
    exstyle = WS_EX_OVERLAPPEDWINDOW;

    if(!AdjustWindowRectEx(&rect, style, 0, exstyle))
    {
        glwtWin32Error("AdjustWindowRect failed");
        goto error;
    }

    nullterm = 0;
    win->win32.hwnd = CreateWindowExW(
        exstyle,
        (LPCWSTR)(intptr_t)glwt.win32.classatom,
        &nullterm,  // window title
        style,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rect.right - rect.left, rect.bottom - rect.top,
        0,
        0,
        glwt.win32.hinstance,
        0);
    if(!win->win32.hwnd)
    {
        glwtWin32Error("CreateWindowExW failed");
        goto error;
    }

    SetWindowLongPtr(win->win32.hwnd, 0, (LONG_PTR)win);

    win->win32.hdc = GetDC(win->win32.hwnd);
    if(!win->win32.hdc)
    {
        glwtWin32Error("GetDC failed");
        goto error;
    }

    if(!SetPixelFormat(
        win->win32.hdc,
#ifdef GLWT_USE_EGL
        glwt.egl.visual_id,
#else
        glwt.wgl.pixel_format,
#endif
        &pfd))
    {
        glwtWin32Error("SetPixelFormat failed");
        goto error;
    }

#ifdef GLWT_USE_EGL
    if(glwtWindowCreateEGL(win, share, win->win32.hwnd))
#else
    if(glwtWindowCreateWGL(win, share))
#endif
        goto error;

    glwtWindowSetTitle(win, title);

    return win;
error:
    glwtWindowDestroy(win);
    return 0;
}

void glwtWindowDestroy(GLWTWindow *win)
{
    if(!win)
        return;

#ifdef GLWT_USE_EGL
    glwtWindowDestroyEGL(win);
#else
    glwtWindowDestroyWGL(win);
#endif

    if(win->win32.hdc)
        ReleaseDC(win->win32.hwnd, win->win32.hdc);

    if(win->win32.hwnd)
        DestroyWindow(win->win32.hwnd);

    free(win);
}

void glwtWindowShow(GLWTWindow *win, int show)
{
    ShowWindow(win->win32.hwnd, show ? SW_SHOWNORMAL : SW_HIDE);
}

void glwtWindowSetTitle(GLWTWindow *win, const char *title)
{
    WCHAR *buffer;
    DWORD len;

    if((len = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, title, -1, NULL, 0)) == 0)
    {
        glwtWin32Error("MultiByteToWideChar failed");
        return;
    }

    buffer = alloca(sizeof(WCHAR) * len);
    MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, title, -1, buffer, len);

    SetWindowTextW(win->win32.hwnd, buffer);
}
