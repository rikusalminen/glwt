#include <string.h>
#include <errno.h>
#include <sys/select.h>

#include <glwt_internal.h>

static int mapKeyMod(unsigned int state)
{
    // TODO: What to do with Mod3Mask?
    return 0
        | ((state & ShiftMask) ? GLWT_MOD_SHIFT : 0)
        | ((state & ControlMask) ? GLWT_MOD_CTRL : 0)
        | ((state & Mod1Mask) ? GLWT_MOD_ALT : 0)
        | ((state & Mod4Mask) ? GLWT_MOD_SUPER : 0)
        | ((state & Mod5Mask) ? GLWT_MOD_ALTGR : 0)
        | ((state & Mod2Mask) ? GLWT_MOD_NUM_LOCK : 0)
        | ((state & LockMask) ? GLWT_MOD_CAPS_LOCK : 0);
}

static int mapButtons(unsigned int state)
{
    return 0
        | ((state & Button1Mask) ? (1 << 0) : 0)
        | ((state & Button2Mask) ? (1 << 1) : 0)
        | ((state & Button3Mask) ? (1 << 2) : 0)
        | ((state & Button4Mask) ? (1 << 3) : 0)
        | ((state & Button5Mask) ? (1 << 4) : 0);
}

static int translate_key(int xkeysym)
{
    switch(xkeysym)
    {
        case XK_BackSpace: return GLWT_KEY_BACKSPACE;
        case XK_Tab: return GLWT_KEY_TAB;
        case XK_Return: return GLWT_KEY_RETURN;
        case XK_Escape: return GLWT_KEY_ESCAPE;
        case XK_space: return GLWT_KEY_SPACE;

        case XK_plus: return GLWT_KEY_PLUS;
        case XK_comma: return GLWT_KEY_COMMA;
        case XK_minus: return GLWT_KEY_MINUS;
        case XK_period: return GLWT_KEY_PERIOD;
        case XK_slash: return GLWT_KEY_SLASH;

        case XK_0: return GLWT_KEY_0;
        case XK_1: return GLWT_KEY_1;
        case XK_2: return GLWT_KEY_2;
        case XK_3: return GLWT_KEY_3;
        case XK_4: return GLWT_KEY_4;
        case XK_5: return GLWT_KEY_5;
        case XK_6: return GLWT_KEY_6;
        case XK_7: return GLWT_KEY_7;
        case XK_8: return GLWT_KEY_8;
        case XK_9: return GLWT_KEY_9;

        case XK_a: return GLWT_KEY_A;
        case XK_b: return GLWT_KEY_B;
        case XK_c: return GLWT_KEY_C;
        case XK_d: return GLWT_KEY_D;
        case XK_e: return GLWT_KEY_E;
        case XK_f: return GLWT_KEY_F;
        case XK_g: return GLWT_KEY_G;
        case XK_h: return GLWT_KEY_H;
        case XK_i: return GLWT_KEY_I;
        case XK_j: return GLWT_KEY_J;
        case XK_k: return GLWT_KEY_K;
        case XK_l: return GLWT_KEY_L;
        case XK_m: return GLWT_KEY_M;
        case XK_n: return GLWT_KEY_N;
        case XK_o: return GLWT_KEY_O;
        case XK_p: return GLWT_KEY_P;
        case XK_q: return GLWT_KEY_Q;
        case XK_r: return GLWT_KEY_R;
        case XK_s: return GLWT_KEY_S;
        case XK_t: return GLWT_KEY_T;
        case XK_u: return GLWT_KEY_U;
        case XK_v: return GLWT_KEY_V;
        case XK_w: return GLWT_KEY_W;
        case XK_x: return GLWT_KEY_X;
        case XK_y: return GLWT_KEY_Y;
        case XK_z: return GLWT_KEY_Z;

        case XK_Delete: return GLWT_KEY_DELETE;

        case XK_KP_Insert: return GLWT_KEY_KEYPAD_0;
        case XK_KP_End: return GLWT_KEY_KEYPAD_1;
        case XK_KP_Down: return GLWT_KEY_KEYPAD_2;
        case XK_KP_Page_Down: return GLWT_KEY_KEYPAD_3;
        case XK_KP_Left: return GLWT_KEY_KEYPAD_4;
        case XK_KP_Begin: return GLWT_KEY_KEYPAD_5;
        case XK_KP_Right: return GLWT_KEY_KEYPAD_6;
        case XK_KP_Home: return GLWT_KEY_KEYPAD_7;
        case XK_KP_Up: return GLWT_KEY_KEYPAD_8;
        case XK_KP_Page_Up: return GLWT_KEY_KEYPAD_9;
        case XK_KP_Delete: return GLWT_KEY_KEYPAD_SEPARATOR;
        case XK_KP_Divide: return GLWT_KEY_KEYPAD_DIVIDE;
        case XK_KP_Multiply: return GLWT_KEY_KEYPAD_MULTIPLY;
        case XK_KP_Add: return GLWT_KEY_KEYPAD_PLUS;
        case XK_KP_Subtract: return GLWT_KEY_KEYPAD_MINUS;
        case XK_KP_Enter: return GLWT_KEY_KEYPAD_ENTER;

        case XK_Up: return GLWT_KEY_UP;
        case XK_Down: return GLWT_KEY_DOWN;
        case XK_Left: return GLWT_KEY_LEFT;
        case XK_Right: return GLWT_KEY_RIGHT;
        case XK_Page_Up: return GLWT_KEY_PAGE_UP;
        case XK_Page_Down: return GLWT_KEY_PAGE_DOWN;
        case XK_Home: return GLWT_KEY_HOME;
        case XK_End: return GLWT_KEY_END;
        case XK_Insert: return GLWT_KEY_INSERT;

        case XK_F1: return GLWT_KEY_F1;
        case XK_F2: return GLWT_KEY_F2;
        case XK_F3: return GLWT_KEY_F3;
        case XK_F4: return GLWT_KEY_F4;
        case XK_F5: return GLWT_KEY_F5;
        case XK_F6: return GLWT_KEY_F6;
        case XK_F7: return GLWT_KEY_F7;
        case XK_F8: return GLWT_KEY_F8;
        case XK_F9: return GLWT_KEY_F9;
        case XK_F10: return GLWT_KEY_F10;
        case XK_F11: return GLWT_KEY_F11;
        case XK_F12: return GLWT_KEY_F12;

        case XK_Shift_L: return GLWT_KEY_LSHIFT;
        case XK_Shift_R: return GLWT_KEY_RSHIFT;
        case XK_Control_L: return GLWT_KEY_LCTRL;
        case XK_Control_R: return GLWT_KEY_RCTRL;
        case XK_Alt_L: return GLWT_KEY_LALT;
        case XK_Alt_R: return GLWT_KEY_RALT;
        case XK_Super_L: return GLWT_KEY_LSUPER;
        case XK_Super_R: return GLWT_KEY_RSUPER;
        case XK_Menu: return GLWT_KEY_MENU;
        case XK_ISO_Level3_Shift: return GLWT_KEY_ALTGR;
        case XK_Num_Lock: return GLWT_KEY_NUM_LOCK;
        case XK_Caps_Lock: return GLWT_KEY_CAPS_LOCK;
        case XK_Scroll_Lock: return GLWT_KEY_SCROLL_LOCK;

        default:
            break;
    }

    return GLWT_KEY_UNKNOWN;
}

static unsigned int decode_utf8(const unsigned char* data, int len)
{
    if(len == 0)
        return 0;
    else if(len == 1 &&
        (data[0] & 0x80) == 0)
        return data[0];
    else if(len == 2 &&
        (data[0] & 0xe0) == 0xc0 &&
        (data[1] & 0xc0) == 0x80)
        return ((data[0] & 0x1f) << 6) | (data[1] & 0x3f);
    else if(len == 3 &&
        (data[0] & 0xf0) == 0xe0 &&
        (data[1] & 0xc0) == 0x80 &&
        (data[2] & 0xc0) == 0x80)
        return ((data[0] & 0x0f) << 12) | ((data[1] & 0x3f) << 6) | (data[2] & 0x3f);
    else if(len == 4 &&
        (data[0] & 0xf7) == 0xf0 &&
        (data[1] & 0x3f) == 0x80 &&
        (data[2] & 0x3f) == 0x80 &&
        (data[3] & 0x3f) == 0x80)
        return ((data[0] & 0x7) << 18) | ((data[1] & 0x3f) << 12) | ((data[2] & 0x3f) << 6) | (data[3] & 0x3f);

    return 0;
}

static Bool xlib_event_predicate(Display *display, XEvent *event, XPointer arg)
{
    (void)display; (void)event; (void)arg;
    return True;
}

static int xlib_handle_event()
{
    XEvent event;
    int num_handled_events = 0;

    while(XCheckIfEvent(glwt.x11.display, &event, xlib_event_predicate, NULL))
    {
        ++num_handled_events;

        GLWTWindow *win = 0;
        if(XFindContext(
                glwt.x11.display,
                event.xany.window,
                glwt.x11.xcontext,
                (XPointer*)&win) != 0)
            continue;

        switch(event.type)
        {
            case ConfigureNotify:
                if(glwt.event_callback)
                {
                    GLWTEvent e;
                    e.window = win;
                    e.type = GLWT_WINDOW_RESIZE;
                    e.resize.width = event.xconfigure.width;
                    e.resize.height = event.xconfigure.height;
                    glwt.event_callback(&e);
                }
                break;
            case MapNotify:
            case UnmapNotify:
                if(glwt.event_callback)
                {
                    GLWTEvent e;
                    e.window = win;
                    e.type = event.type == MapNotify ? GLWT_WINDOW_SHOW : GLWT_WINDOW_HIDE;
                    e.dummy.dummy  = 0;
                    glwt.event_callback(&e);
                }
                break;
            case Expose:
                if(glwt.event_callback)
                {
                    GLWTEvent e;
                    e.window = win;
                    e.type = GLWT_WINDOW_EXPOSE;
                    e.dummy.dummy = 0;
                    glwt.event_callback(&e);
                }
                break;
            case KeyPress:
            case KeyRelease:
                if(glwt.event_callback)
                {
                    GLWTEvent e;
                    e.window = win;
                    e.type = event.type == KeyPress ?
                        GLWT_KEY_PRESS : GLWT_KEY_RELEASE;
                    e.key.keysym = translate_key(XkbKeycodeToKeysym(glwt.x11.display, event.xkey.keycode, 0, 0));
                    e.key.scancode = event.xkey.keycode;
                    e.key.mod = mapKeyMod(event.xkey.state);
                    glwt.event_callback(&e);
                }

                if(event.type == KeyPress && !XFilterEvent(&event, None))
                {
                    char buffer[5] = { 0, 0, 0, 0, 0 };
                    int buffer_size = sizeof(buffer);
                    int len = 0;
                    Status status = XLookupNone;

                    len = Xutf8LookupString(
                        win->x11.xic,
                        (XKeyEvent*)&event.xkey,
                        buffer, buffer_size-1,
                        NULL,
                        &status);
                    buffer[len] = 0;

                    if(status == XBufferOverflow)
                    {
                        glwtErrorPrintf("Xutf8LookupString buffer overflow");
                        return -1;
                    }

                    if(glwt.event_callback &&
                        (status == XLookupBoth || status == XLookupChars))
                    {
                        GLWTEvent e;
                        e.window = win;
                        e.type = GLWT_CHARACTER_INPUT;
                        e.character.unicode = decode_utf8((unsigned char*)buffer, len);

                        glwt.event_callback(&e);
                    }
                }

                break;
            case FocusIn:
            case FocusOut:
                if(glwt.event_callback)
                {
                    GLWTEvent e;
                    e.window = win;
                    e.type = event.type == FocusIn ? GLWT_WINDOW_FOCUS_IN : GLWT_WINDOW_FOCUS_OUT;
                    e.dummy.dummy = 0;
                    glwt.event_callback(&e);
                }
                break;
            case ButtonPress:
            case ButtonRelease:
                if(glwt.event_callback)
                {
                    GLWTEvent e;
                    e.window = win;
                    e.type = event.type == ButtonPress ?
                        GLWT_BUTTON_PRESS : GLWT_BUTTON_RELEASE;
                    e.button.x = event.xbutton.x;
                    e.button.y = event.xbutton.y;
                    e.button.button = event.xbutton.button; // todo: make these consistent on different platforms
                    e.button.mod = mapKeyMod(event.xbutton.state);
                    glwt.event_callback(&e);
                }
                break;
            case MotionNotify:
                if(glwt.event_callback)
                {
                    GLWTEvent e;
                    e.window = win;
                    e.type = GLWT_MOUSE_MOTION;
                    e.motion.x = event.xmotion.x;
                    e.motion.y = event.xmotion.y;
                    e.motion.buttons = mapButtons(event.xmotion.state);
                    glwt.event_callback(&e);
                }
                break;
            case EnterNotify:
            case LeaveNotify:
                if(glwt.event_callback)
                {
                    GLWTEvent e;
                    e.window = win;
                    e.type = event.type == EnterNotify ? GLWT_WINDOW_MOUSE_ENTER : GLWT_WINDOW_MOUSE_LEAVE;
                    e.dummy.dummy = 0;
                    glwt.event_callback(&e);
                }
                break;
            case ClientMessage:
                if((Atom)event.xclient.data.l[0] == glwt.x11.atoms.WM_DELETE_WINDOW)
                {
                    win->closed = 1;

                    if(glwt.event_callback)
                    {
                        GLWTEvent e;
                        e.window = win;
                        e.type = GLWT_WINDOW_CLOSE;
                        e.dummy.dummy = 0;
                        glwt.event_callback(&e);
                    }
                } else if((Atom)event.xclient.data.l[0] == glwt.x11.atoms._NET_WM_PING)
                {
                    event.xclient.window = RootWindow(glwt.x11.display, glwt.x11.screen_num);
                    XSendEvent(
                        glwt.x11.display,
                        event.xclient.window,
                        False,
                       SubstructureNotifyMask | SubstructureRedirectMask,
                       &event);
                }
                break;
            default:
                break;
        }
    }

    return num_handled_events;
}

int glwtEventHandle(int wait)
{
    int handled = 0;
    do
    {
        XFlush(glwt.x11.display);

        handled = xlib_handle_event();
        if(handled < 0)
            return -1;

        if(wait && handled == 0)
        {
            int fd;
            fd_set fds;

            fd = ConnectionNumber(glwt.x11.display);

            FD_ZERO(&fds);
            FD_SET(fd, &fds);

            int val = select(fd + 1, &fds, NULL, NULL, NULL);
            if(val == -1)
            {
                glwtErrorPrintf("select failed: %s", strerror(errno));
                return -1;
            } else if(val == 0)
                return 0;
        }
    } while(handled == 0 && wait);

    return 0;
}
