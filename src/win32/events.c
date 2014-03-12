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
            case VK_INSERT: return GLWT_KEY_KEYPAD_0;
            case VK_END: return GLWT_KEY_KEYPAD_1;
            case VK_DOWN: return GLWT_KEY_KEYPAD_2;
            case VK_NEXT: return GLWT_KEY_KEYPAD_3;
            case VK_LEFT: return GLWT_KEY_KEYPAD_4;
            case VK_CLEAR: return GLWT_KEY_KEYPAD_5;
            case VK_RIGHT: return GLWT_KEY_KEYPAD_6;
            case VK_HOME: return GLWT_KEY_KEYPAD_7;
            case VK_UP: return GLWT_KEY_KEYPAD_8;
            case VK_PRIOR: return GLWT_KEY_KEYPAD_9;
            case VK_DIVIDE: return GLWT_KEY_KEYPAD_DIVIDE;
            case VK_MULTIPLY: return GLWT_KEY_KEYPAD_MULTIPLY;
            case VK_SUBTRACT: return GLWT_KEY_KEYPAD_MINUS;
            case VK_ADD: return GLWT_KEY_KEYPAD_PLUS;
            case VK_DELETE: return GLWT_KEY_KEYPAD_SEPARATOR;
            default:
                break;
        }
    }

    switch(wParam)
    {
        case VK_SHIFT:
            return ((unsigned)((lParam & 0x01ff0000) >> 16) == MapVirtualKeyW(VK_RSHIFT, MAPVK_VK_TO_VSC)) ?
                GLWT_KEY_RSHIFT : GLWT_KEY_LSHIFT;
        case VK_CONTROL:
            return (lParam & 0x01000000) ? GLWT_KEY_RCTRL : GLWT_KEY_LCTRL;
        case VK_MENU:
            return (lParam & 0x01000000) ? GLWT_KEY_RALT : GLWT_KEY_LALT;
        case VK_RETURN:
            return (lParam & 0x01000000) ? GLWT_KEY_KEYPAD_ENTER : GLWT_KEY_RETURN;

        case VK_BACK: return GLWT_KEY_BACKSPACE;
        case VK_TAB: return GLWT_KEY_TAB;
        case VK_ESCAPE: return GLWT_KEY_ESCAPE;
        case VK_SPACE: return GLWT_KEY_SPACE;

        case VK_OEM_PLUS: return GLWT_KEY_PLUS;
        case VK_OEM_COMMA: return GLWT_KEY_COMMA;
        case VK_OEM_MINUS: return GLWT_KEY_MINUS;
        case VK_OEM_PERIOD: return GLWT_KEY_PERIOD;
        case VK_DIVIDE: return GLWT_KEY_SLASH;

        case (int)('0'): return GLWT_KEY_0;
        case (int)('1'): return GLWT_KEY_1;
        case (int)('2'): return GLWT_KEY_2;
        case (int)('3'): return GLWT_KEY_3;
        case (int)('4'): return GLWT_KEY_4;
        case (int)('5'): return GLWT_KEY_5;
        case (int)('6'): return GLWT_KEY_6;
        case (int)('7'): return GLWT_KEY_7;
        case (int)('8'): return GLWT_KEY_8;
        case (int)('9'): return GLWT_KEY_9;

        case (int)('A'): return GLWT_KEY_A;
        case (int)('B'): return GLWT_KEY_B;
        case (int)('C'): return GLWT_KEY_C;
        case (int)('D'): return GLWT_KEY_D;
        case (int)('E'): return GLWT_KEY_E;
        case (int)('F'): return GLWT_KEY_F;
        case (int)('G'): return GLWT_KEY_G;
        case (int)('H'): return GLWT_KEY_H;
        case (int)('I'): return GLWT_KEY_I;
        case (int)('J'): return GLWT_KEY_J;
        case (int)('K'): return GLWT_KEY_K;
        case (int)('L'): return GLWT_KEY_L;
        case (int)('M'): return GLWT_KEY_M;
        case (int)('N'): return GLWT_KEY_N;
        case (int)('O'): return GLWT_KEY_O;
        case (int)('P'): return GLWT_KEY_P;
        case (int)('Q'): return GLWT_KEY_Q;
        case (int)('R'): return GLWT_KEY_R;
        case (int)('S'): return GLWT_KEY_S;
        case (int)('T'): return GLWT_KEY_T;
        case (int)('U'): return GLWT_KEY_U;
        case (int)('V'): return GLWT_KEY_V;
        case (int)('W'): return GLWT_KEY_W;
        case (int)('X'): return GLWT_KEY_X;
        case (int)('Y'): return GLWT_KEY_Y;
        case (int)('Z'): return GLWT_KEY_Z;

        case VK_DELETE: return GLWT_KEY_DELETE;

        case VK_NUMPAD0: return GLWT_KEY_KEYPAD_0;
        case VK_NUMPAD1: return GLWT_KEY_KEYPAD_1;
        case VK_NUMPAD2: return GLWT_KEY_KEYPAD_2;
        case VK_NUMPAD3: return GLWT_KEY_KEYPAD_3;
        case VK_NUMPAD4: return GLWT_KEY_KEYPAD_4;
        case VK_NUMPAD5: return GLWT_KEY_KEYPAD_5;
        case VK_NUMPAD6: return GLWT_KEY_KEYPAD_6;
        case VK_NUMPAD7: return GLWT_KEY_KEYPAD_7;
        case VK_NUMPAD8: return GLWT_KEY_KEYPAD_8;
        case VK_NUMPAD9: return GLWT_KEY_KEYPAD_9;
        case VK_DECIMAL: return GLWT_KEY_KEYPAD_SEPARATOR;
        case VK_MULTIPLY: return GLWT_KEY_KEYPAD_MULTIPLY;
        case VK_ADD: return GLWT_KEY_KEYPAD_PLUS;
        case VK_SUBTRACT: return GLWT_KEY_KEYPAD_MINUS;

        case VK_UP: return GLWT_KEY_UP;
        case VK_DOWN: return GLWT_KEY_DOWN;
        case VK_LEFT: return GLWT_KEY_LEFT;
        case VK_RIGHT: return GLWT_KEY_RIGHT;
        case VK_PRIOR: return GLWT_KEY_PAGE_UP;
        case VK_NEXT: return GLWT_KEY_PAGE_DOWN;
        case VK_HOME: return GLWT_KEY_HOME;
        case VK_END: return GLWT_KEY_END;
        case VK_INSERT: return GLWT_KEY_INSERT;

        case VK_F1: return GLWT_KEY_F1;
        case VK_F2: return GLWT_KEY_F2;
        case VK_F3: return GLWT_KEY_F3;
        case VK_F4: return GLWT_KEY_F4;
        case VK_F5: return GLWT_KEY_F5;
        case VK_F6: return GLWT_KEY_F6;
        case VK_F7: return GLWT_KEY_F7;
        case VK_F8: return GLWT_KEY_F8;
        case VK_F9: return GLWT_KEY_F9;
        case VK_F10: return GLWT_KEY_F10;
        case VK_F11: return GLWT_KEY_F11;
        case VK_F12: return GLWT_KEY_F12;

        case VK_LSHIFT: return GLWT_KEY_LSHIFT;
        case VK_RSHIFT: return GLWT_KEY_RSHIFT;
        case VK_LCONTROL: return GLWT_KEY_LCTRL;
        case VK_RCONTROL: return GLWT_KEY_RCTRL;
        case VK_LMENU: return GLWT_KEY_LALT;
        case VK_RMENU: return GLWT_KEY_RALT;
        case VK_LWIN: return GLWT_KEY_LSUPER;
        case VK_RWIN: return GLWT_KEY_RSUPER;
        case VK_NUMLOCK: return GLWT_KEY_NUM_LOCK;
        case VK_CAPITAL: return GLWT_KEY_CAPS_LOCK;
        case VK_SCROLL: return GLWT_KEY_SCROLL_LOCK;

        default:
            break;
    }

    return GLWT_KEY_UNKNOWN;
}

static unsigned int utf16_decode(unsigned int utf16)
{
    unsigned short lo = utf16 & 0xFFFF;
    unsigned short hi = (utf16 & 0xFFFF0000) >> 16;

    if(lo < 0xD8000 || lo > 0xDFFF) return lo;
    else if(lo < 0xD800 || lo > 0xDBFF) return 0;
    else if(hi < 0xDC00 || hi > 0xDFFF) return 0;
    else return ((lo & 0x3FF) << 10) | (hi & 0x3FF) + 0x10000;
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
                return 0;

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

                return 0;

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

                return 0;

            case WM_SETFOCUS:
            case WM_KILLFOCUS:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = uMsg == WM_SETFOCUS ? GLWT_WINDOW_FOCUS_IN : GLWT_WINDOW_FOCUS_OUT;
                    event.dummy.dummy = 0;
                    win->win_callback(win, &event, win->userdata);
                }

                return 0;

            case WM_MOUSEHOVER:
            case WM_MOUSELEAVE:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = (uMsg == WM_MOUSEHOVER ? GLWT_WINDOW_MOUSE_ENTER : GLWT_WINDOW_MOUSE_LEAVE);
                    event.dummy.dummy = 0;
                    win->win_callback(win, &event, win->userdata);
                }

                win->win32.hover = (uMsg == WM_MOUSEHOVER ? 1 : 0);

                return 0;

            case WM_SHOWWINDOW:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = (wParam == TRUE ? GLWT_WINDOW_SHOW : GLWT_WINDOW_HIDE);
                    event.dummy.dummy = 0;
                    win->win_callback(win, &event, win->userdata);
                }

                return 0;

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

                return 0;

            case WM_CHAR:
                if(win->win_callback)
                {
                    GLWTWindowEvent event;
                    event.window = win;
                    event.type = GLWT_WINDOW_CHARACTER_INPUT;
                    event.character.unicode = utf16_decode((unsigned int)wParam);

                    win->win_callback(win, &event, win->userdata);
                }

                return 0;

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
