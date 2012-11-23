#ifndef GLWT_glwt_x11_h
#define GLWT_glwt_x11_h

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/XKBlib.h>

#include <x11/keymap.h>

#define GLWT_X11_ATOMS(ATOM) \
    ATOM(_NET_SUPPORTED) \
    ATOM(WM_DELETE_WINDOW) \
    ATOM(_NET_WM_PING) \
    ATOM(_NET_WM_NAME) \
    ATOM(UTF8_STRING)

struct glwt_x11
{
    Display *display;
    XContext xcontext;
    int screen_num;
    Visual *visual;
    int depth;
    int visual_id;
    Colormap colormap;

    struct
    {
#define GLWT_X11_ATOM_DECLARE(X) Atom X;
        GLWT_X11_ATOMS(GLWT_X11_ATOM_DECLARE)
#undef GLWT_X11_ATOM_DECLARE
    } atoms;

    struct keymap keymap;
};

struct glwt_window_x11
{
    Window window;
};

#endif
