#include <windows.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

void glwtWin32Error(const char *msg)
{
   LPVOID msgbuf = 0;
   DWORD err = GetLastError();

    if(!FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        err,
        0,
        (LPSTR) &msgbuf,
        0, NULL ))
    {
        glwtErrorPrintf("%s: %d\n", msg, err);
        return;
    }

    glwtErrorPrintf("%s: %s\n", msg, (const char *)msgbuf);
    LocalFree(msgbuf);
}

int glwtInit(
    const GLWTConfig *config,
    void (*error_callback)(const char *msg, void *userdata),
    void *userdata)
{
    WCHAR klassname[] = { 'G', 'L', 'W', 'T', 0 };
    int fmt;
    WNDCLASSEXW klass;
    PIXELFORMATDESCRIPTOR pfd;

    glwt.error_callback = error_callback;
    glwt.userdata = userdata;

    glwt.win32.hinstance = GetModuleHandle(0);
    if(!glwt.win32.hinstance)
    {
        glwtWin32Error("GetModuleHandle failed");
        goto error;
    }

    klass.cbSize = sizeof(WNDCLASSEXW);
    klass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    klass.lpfnWndProc = &glwtWin32WndProc;
    klass.cbClsExtra = 0;
    klass.cbWndExtra = sizeof(GLWTWindow*);
    klass.hInstance = glwt.win32.hinstance;
    klass.hIcon = 0;
    klass.hCursor = LoadCursor( NULL, IDC_ARROW );
    klass.hbrBackground = 0;
    klass.lpszMenuName = 0;
    klass.lpszClassName = klassname;
    klass.hIconSm = 0;

    glwt.win32.classatom = RegisterClassExW(&klass);
    if(!glwt.win32.classatom)
        glwtWin32Error("RegisterWindowClassExW failed");

    glwt.win32.dummy_hwnd = CreateWindowExW(
        0,
        (LPCWSTR)(intptr_t)glwt.win32.classatom,
        L"",    // window title
        WS_POPUP | WS_DISABLED,
        CW_USEDEFAULT, CW_USEDEFAULT,
        100, 100, /* width, height */
        0,
        0,
        glwt.win32.hinstance,
        0);

    glwt.win32.dummy_hdc = GetDC(glwt.win32.dummy_hwnd);
    if(!glwt.win32.dummy_hdc)
    {
        glwtWin32Error("GetDC failed");
        return -1;
    }

    memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;

    fmt = ChoosePixelFormat(glwt.win32.dummy_hdc, &pfd);
    if(!fmt)
    {
        glwtWin32Error("ChoosePixelFormat failed");
        goto error;
    }

    if(!SetPixelFormat(glwt.win32.dummy_hdc, fmt, &pfd))
    {
        glwtWin32Error("SetPixelFormat failed");
        goto error;
    }

#ifdef GLWT_USE_EGL
    glwt.egl.display = eglGetDisplay(glwt.win32.dummy_hdc);
    if(glwtInitEGL(config) == -1)
#else
    if(glwtInitWGL(config) == -1)
#endif
        goto error;

    return 0;
error:
    glwtQuit();
    return -1;
}

void glwtQuit()
{
#ifdef GLWT_USE_EGL
    glwtQuitEGL();
#else
    glwtQuitWGL();
#endif

    if(glwt.win32.dummy_hdc)
        ReleaseDC(glwt.win32.dummy_hwnd, glwt.win32.dummy_hdc);
    if(glwt.win32.dummy_hwnd)
        DestroyWindow(glwt.win32.dummy_hwnd);

    if(glwt.win32.classatom &&
        !UnregisterClassW((LPCWSTR)(intptr_t)glwt.win32.classatom, glwt.win32.hinstance))
        glwtWin32Error("UnregisterClassW failed");

    memset(&glwt, 0, sizeof(struct glwt));
}
