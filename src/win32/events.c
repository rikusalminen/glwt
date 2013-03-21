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

static int translate_key(WPARAM wParam, LPARAM lParam)
{
    if (!(HIWORD(lParam) & 0x100))    // Handle Keypad and Numlock
    {
        switch(MapVirtualKeyW(HIWORD(lParam) & 0xFF, MAPVK_VSC_TO_VK))
        {
            case VK_INSERT:
                return GLWT_KEY_KEYPAD_0;
            case VK_END:
                return GLWT_KEY_KEYPAD_1;
            case VK_DOWN:
                return GLWT_KEY_KEYPAD_2;
            case VK_NEXT:
                return GLWT_KEY_KEYPAD_3;
            case VK_LEFT:
                return GLWT_KEY_KEYPAD_4;
            case VK_CLEAR:
                return GLWT_KEY_KEYPAD_5;
            case VK_RIGHT:
                return GLWT_KEY_KEYPAD_6;
            case VK_HOME:
                return GLWT_KEY_KEYPAD_7;
            case VK_UP:
                return GLWT_KEY_KEYPAD_8;
            case VK_PRIOR:
                return GLWT_KEY_KEYPAD_9;
            case VK_DIVIDE:
                return GLWT_KEY_KEYPAD_DIVIDE;
            case VK_MULTIPLY:
                return GLWT_KEY_KEYPAD_MULTIPLY;
            case VK_SUBTRACT:
                return GLWT_KEY_KEYPAD_MINUS;
            case VK_ADD:
                return GLWT_KEY_KEYPAD_PLUS;
            case VK_DELETE:
                return GLWT_KEY_KEYPAD_SEPARATOR;
            default:
                break;
        }
    }

    switch(wParam)
    {
        case VK_SHIFT:
            return (((lParam & 0x01ff0000) >> 16) == MapVirtualKeyW(VK_RSHIFT, MAPVK_VK_TO_VSC)) ?
                GLWT_KEY_RSHIFT : GLWT_KEY_LSHIFT;
        case VK_CONTROL:
            return (lParam & 0x01000000) ? GLWT_KEY_RCTRL : GLWT_KEY_LCTRL;
        case VK_MENU:
            return (lParam & 0x01000000) ? GLWT_KEY_RALT : GLWT_KEY_LALT;
        case VK_RETURN:
            return (lParam & 0x01000000) ? GLWT_KEY_KEYPAD_ENTER : GLWT_KEY_RETURN;
        default:
            return keymap_lookup(&glwt.win32.keymap, (int)wParam);
    }

    return GLWT_KEY_UNKNOWN;
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
            case WM_KEYDOWN:
            case WM_SYSKEYUP:
            case WM_SYSKEYDOWN:
				if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN) ? GLWT_WINDOW_KEY_DOWN : GLWT_WINDOW_KEY_UP;
                    event.key.keysym = translate_key(wParam, lParam);
                    event.key.scancode = (lParam >> 16) & 0xff;
                    event.key.mod = GetModifiers();
                    win->win_callback(win, &event, win->userdata);
                }

                if(uMsg == WM_SYSKEYUP || uMsg == WM_SYSKEYDOWN)
                    break;
                return 0;

            case WM_LBUTTONUP:
            case WM_LBUTTONDOWN:
            case WM_MBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_XBUTTONUP:
            case WM_XBUTTONDOWN:
            case WM_MOUSEWHEEL:
            case WM_MOUSEHWHEEL:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;

                    event.window = win;
                    event.type = (
                        uMsg == WM_LBUTTONUP ||
                        uMsg == WM_MBUTTONUP ||
                        uMsg == WM_RBUTTONUP ||
                        uMsg == WM_XBUTTONUP ||
                        uMsg == WM_MOUSEWHEEL ||
                        uMsg == WM_MOUSEHWHEEL
                        ) ? GLWT_WINDOW_BUTTON_DOWN : GLWT_WINDOW_BUTTON_UP;
                    event.button.x = LOWORD(lParam);
                    event.button.y = HIWORD(lParam);
                    event.button.button =
                        (uMsg == WM_LBUTTONUP || uMsg == WM_LBUTTONDOWN) ? 0 :
                        (uMsg == WM_MBUTTONUP || uMsg == WM_MBUTTONDOWN) ? 1 :
                        (uMsg == WM_RBUTTONUP || uMsg == WM_RBUTTONDOWN) ? 2 :
                        (uMsg == WM_XBUTTONUP || uMsg == WM_XBUTTONDOWN) ? (HIWORD(wParam) == XBUTTON1 ? 5 : 6) :
                        (uMsg == WM_MOUSEWHEEL) ? (GET_WHEEL_DELTA_WPARAM(wParam) < 0 ? 3 : 4) :
                        (uMsg == WM_MOUSEHWHEEL) ? (GET_WHEEL_DELTA_WPARAM(wParam) < 0 ? 7 : 8) :
                        0;

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
                        (wParam & MK_LBUTTON  ? (1 << 0) : 0) |
                        (wParam & MK_RBUTTON  ? (1 << 1) : 0) |
                        (wParam & MK_MBUTTON  ? (1 << 2) : 0) |
                        (wParam & MK_XBUTTON1 ? (1 << 5) : 0) |
                        (wParam & MK_XBUTTON2 ? (1 << 6) : 0);
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

                return 0;

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

    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
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
