#include <glwt_internal.h>

#include <windows.h>
#include <windowsx.h> /* GET_X_LPARAM, GET_Y_LPARAM macros */

static int GetModifiers()
{
    return
        (GetKeyState(VK_SHIFT) & 0x8000 ? GLWT_MOD_SHIFT : 0) |
        (GetKeyState(VK_CONTROL) & 0x8000 ? GLWT_MOD_CTRL : 0) |
        (GetKeyState(VK_LMENU) & 0x8000 ? GLWT_MOD_ALT : 0) |
        (GetKeyState(VK_RMENU) & 0x8000 ? GLWT_MOD_ALTGR : 0) |
        (GetKeyState(VK_NUMLOCK) & 0x8000 ? GLWT_MOD_NUM_LOCK : 0) |
        (GetKeyState(VK_CAPITAL) & 0x8000 ? GLWT_MOD_CAPS_LOCK : 0) |
        (GetKeyState(VK_LWIN) & 0x8000 ? GLWT_MOD_SUPER : 0) |
        (GetKeyState(VK_RWIN) & 0x8000 ? GLWT_MOD_SUPER : 0);
}

LRESULT CALLBACK glwtWin32WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    GLWTWindow *win = (GLWTWindow*)GetWindowLongPtr(hwnd, 0);

    if(win)
    {
        switch(uMsg)
        {
            case WM_PAINT:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = GLWT_WINDOW_EXPOSE;
                    event.dummy.dummy = 0;
                    win->win_callback(win, &event, win->userdata);
                }

                ValidateRect(hwnd, NULL);
                return 1;

            case WM_KEYUP:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = GLWT_WINDOW_KEY_UP;
                    event.key.keysym = keymap_lookup(&glwt.win32.keymap, wParam);
                    event.key.scancode = (lParam >> 16) & 0xff;
                    event.key.mod = GetModifiers();
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_KEYDOWN:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = GLWT_WINDOW_KEY_DOWN;
                    event.key.keysym = keymap_lookup(&glwt.win32.keymap, wParam);
                    event.key.scancode = (lParam >> 16) & 0xff;
                    event.key.mod = GetModifiers();
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_LBUTTONUP:
            case WM_LBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_XBUTTONUP:
            case WM_XBUTTONDOWN:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    int button;

                    if(uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDOWN) button = 0;
                    else if(uMsg == WM_MBUTTONUP || uMsg == WM_MBUTTONDOWN) button = 1;
                    else if(uMsg == WM_RBUTTONUP || uMsg == WM_RBUTTONDOWN) button = 2;
                    else button = (wParam & MK_XBUTTON2 ? 4 : 3);

                    event.window = win;
                    event.type =
                        (uMsg == WM_LBUTTONUP || uMsg == WM_MBUTTONUP || uMsg == WM_RBUTTONUP || uMsg == WM_XBUTTONUP)
                            ? GLWT_WINDOW_BUTTON_UP : GLWT_WINDOW_BUTTON_DOWN;
                    event.button.x = LOWORD(lParam);
                    event.button.y = HIWORD(lParam);
                    event.button.button = button;
                    event.button.mod = GetModifiers();
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_MOUSEMOVE:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;

                    /* Keep mouse enter/leave events alive */
                    if( win->win32.hover == 0 )
                    {
                        TRACKMOUSEEVENT tme;
                        tme.cbSize = sizeof(tme);
                        tme.dwFlags = TME_HOVER|TME_LEAVE;
                        tme.hwndTrack = hwnd;
                        tme.dwHoverTime = 1;
                        TrackMouseEvent(&tme);

                        win->win32.hover = -1;
                    }

                    event.window = win;
                    event.type = GLWT_WINDOW_MOUSE_MOTION;
                    event.motion.x = GET_X_LPARAM(lParam);
                    event.motion.y = GET_Y_LPARAM(lParam);
                    event.motion.buttons  =
                        (wParam & MK_LBUTTON  ?  1 : 0) |
                        (wParam & MK_RBUTTON  ?  2 : 0) |
                        (wParam & MK_MBUTTON  ?  4 : 0) |
                        (wParam & MK_XBUTTON1 ?  8 : 0) |
                        (wParam & MK_XBUTTON2 ? 16 : 0);
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_SETFOCUS:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = GLWT_WINDOW_FOCUS_IN;
                    event.dummy.dummy = 0;
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_KILLFOCUS:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = GLWT_WINDOW_FOCUS_OUT;
                    event.dummy.dummy = 0;
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_MOUSEHOVER:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = (win->win32.hover == 1 ? GLWT_WINDOW_NO_EVENT : GLWT_WINDOW_MOUSE_ENTER);
                    event.dummy.dummy = 0;
                    win->win32.hover = 1;
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_MOUSELEAVE:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = GLWT_WINDOW_MOUSE_LEAVE;
                    event.dummy.dummy = 0;
                    win->win32.hover = 0;
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_SHOWWINDOW:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = (wParam == TRUE ? GLWT_WINDOW_SHOW : GLWT_WINDOW_HIDE);
                    event.dummy.dummy = 0;
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_SIZE:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = GLWT_WINDOW_RESIZE;
                    event.resize.width = LOWORD(lParam);
                    event.resize.height = HIWORD(lParam);
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_CLOSE:
                win->closed = 1;

                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = GLWT_WINDOW_CLOSE;
                    event.dummy.dummy = 0;
                    win->win_callback(win, &event, win->userdata);
                }

                return 1;

            case WM_CHAR:
                // TODO: Handle text input

            case WM_DESTROY:
            case WM_QUIT:
                // TODO: Handle destroy and quit messages

            default:
                break;

        }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
