#include <string.h>
#include <errno.h>
#include <sys/select.h>

#include <glwt_internal.h>

static int xlib_handle_event()
{
    XEvent event;

    while(XCheckMaskEvent(glwt.x11.display, ~0, &event) ||
        XCheckTypedEvent(glwt.x11.display, ClientMessage, &event))
    {
        GLWTWindow *win = 0;
        if(XFindContext(glwt.x11.display, event.xany.window, glwt.x11.xcontext, (XPointer*)&win) != 0 ||
            !win)
        {
            glwtErrorPrintf("XFindContext window not found");
            return -1;
        }

        switch(event.type)
        {
            case KeyPress:
                glwtErrorPrintf("KEYPRESS!!!1!");
                win->closed = 1;
                break;
            default:
                break;
        }
    }

    return 0;
}

int glwtEventHandle(int wait)
{
    if(wait)
    {
        int fd;
        fd_set fds;

        fd = ConnectionNumber(glwt.x11.display);

        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        XFlush(glwt.x11.display);

        int val = select(fd + 1, &fds, NULL, NULL, NULL);
        if(val == -1)
        {
            glwtErrorPrintf("select failed: %s", strerror(errno));
            return -1;
        } else if(val == 0)
            return 0;
    }

    return xlib_handle_event();
}
