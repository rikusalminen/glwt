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

static int xlib_handle_event()
{
    XEvent event;
    int num_handled_events = 0;

    while(XCheckMaskEvent(glwt.x11.display, ~0, &event) ||
        XCheckTypedEvent(glwt.x11.display, ClientMessage, &event))
    {
        ++num_handled_events;

        GLWTWindow *win = 0;
        if(XFindContext(glwt.x11.display, event.xany.window, glwt.x11.xcontext, (XPointer*)&win) != 0 ||
            !win)
        {
            glwtErrorPrintf("XFindContext window not found");
            return -1;
        }

        switch(event.type)
        {
            case ConfigureNotify:
                if(win->win_callback)
                {
                    GLWTWindowEvent e = {
                        win,
                        GLWT_WINDOW_RESIZE,
                        .resize = {
                            event.xconfigure.width,
                            event.xconfigure.height
                        }
                    };
                    win->win_callback(win, &e, win->userdata);
                }
                break;
            case MapNotify:
            case UnmapNotify:
                if(win->win_callback)
                {
                    GLWTWindowEvent e = {
                        win,
                        event.type == MapNotify ? GLWT_WINDOW_SHOW : GLWT_WINDOW_HIDE,
                        .dummy = { 0 }
                    };
                    win->win_callback(win, &e, win->userdata);
                }
                break;
            case Expose:
                if(win->win_callback)
                {
                    GLWTWindowEvent e = {
                        win,
                        GLWT_WINDOW_EXPOSE,
                        .dummy = { 0 }
                    };
                    win->win_callback(win, &e, win->userdata);
                }
                break;
            case KeyPress:
            case KeyRelease:
                if(win->win_callback)
                {
                    GLWTWindowEvent e = {
                        win,
                        event.type == KeyPress ? GLWT_WINDOW_KEY_DOWN : GLWT_WINDOW_KEY_UP,
                        .key = {
                            keymap_lookup(
                                &glwt.x11.keymap,
                                XkbKeycodeToKeysym(glwt.x11.display, event.xkey.keycode, 0, 0)),
                            event.xkey.keycode,
                            mapKeyMod(event.xkey.state)
                        }
                    };
                    win->win_callback(win, &e, win->userdata);
                }
                break;
            case FocusIn:
            case FocusOut:
                if(win->win_callback)
                {
                    GLWTWindowEvent e = {
                        win,
                        event.type == FocusIn ? GLWT_WINDOW_FOCUS_IN : GLWT_WINDOW_FOCUS_OUT,
                        .dummy = { 0 }
                    };
                    win->win_callback(win, &e, win->userdata);
                }
                break;
            case ButtonPress:
            case ButtonRelease:
                if(win->win_callback)
                {
                    GLWTWindowEvent e = {
                        win,
                        event.type == ButtonPress ? GLWT_WINDOW_BUTTON_DOWN : GLWT_WINDOW_BUTTON_UP,
                        .button = {
                            event.xbutton.x, event.xbutton.y,
                            event.xbutton.button, // todo: make these consistent on different platforms
                            mapKeyMod(event.xbutton.state)
                        }
                    };
                    win->win_callback(win, &e, win->userdata);
                }
                break;
            case MotionNotify:
                if(win->win_callback)
                {
                    GLWTWindowEvent e = {
                        win,
                        GLWT_WINDOW_MOUSE_MOTION,
                        .motion = {
                            event.xmotion.x, event.xmotion.y,
                            mapButtons(event.xmotion.state)
                        }
                    };
                    win->win_callback(win, &e, win->userdata);
                }
                break;
            case EnterNotify:
            case LeaveNotify:
                if(win->win_callback)
                {
                    GLWTWindowEvent e = {
                        win,
                        event.type == EnterNotify ? GLWT_WINDOW_MOUSE_ENTER : GLWT_WINDOW_MOUSE_LEAVE,
                        .dummy = { 0 }
                    };
                    win->win_callback(win, &e, win->userdata);
                }
                break;
            case ClientMessage:
                if((Atom)event.xclient.data.l[0] == glwt.x11.atoms.WM_DELETE_WINDOW)
                {
                    win->closed = 1;

                    if(win->win_callback)
                    {
                        GLWTWindowEvent e = {
                            win,
                            GLWT_WINDOW_CLOSE,
                            .dummy = { 0 }
                        };
                        win->win_callback(win, &e, win->userdata);
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
