#include <glwt_internal.h>

LRESULT CALLBACK glwtWin32WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    GLWTWindow *win = (GLWTWindow*)GetWindowLongPtr(hwnd, 0);

    if(win)
    {
        switch(message)
        {
            case WM_CLOSE:
                win->closed = 1;
                return 1;
            case WM_PAINT:
                ValidateRect(hwnd, NULL);
                return 1;
            case WM_SETFOCUS:
            case WM_KILLFOCUS:
                return 1;
            case WM_SIZE:
                return 1;
            case WM_MOUSEMOVE:
                return 1;
            case WM_LBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_XBUTTONDOWN:
            case WM_XBUTTONUP:
            case WM_MOUSEWHEEL:
#ifdef WM_MOUSEHWHEEL
            case WM_MOUSEHWHEEL:
#endif
                return 1;
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
                return 1;
            case WM_CHAR:
                // TODO: character events
            case WM_ENTERSIZEMOVE:
            case WM_EXITSIZEMOVE:
            case WM_GETMINMAXINFO:
            case WM_ERASEBKGND:
                // TODO: use these event to deal with window resizing and size limits
            case WM_MOUSELEAVE:
                // TODO: mouseover events
            default:
               break;
        }
    }

    return DefWindowProc(hwnd, message, wparam, lparam);
}

int glwtEventHandle(int wait)
{
    while(1)
    {
        MSG msg;

        if(wait)
        {
            if(GetMessageW(&msg, 0, 0, 0) == -1)
            {
                glwtWin32Error("GetMesssageW failed");
                return -1;
            }

            wait = 0;
        } else
        {
            if(!PeekMessageW(&msg, 0, 0, 0, PM_REMOVE))
                break;
        }

        TranslateMessage(&msg);

        if(DispatchMessageW(&msg) < 0)
        {
            glwtWin32Error("DispatchMessageW failed");
            return -1;
        }
    }

    return 0;
}
