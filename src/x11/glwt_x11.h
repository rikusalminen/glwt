#ifndef GLWT_glwt_x11_h
#define GLWT_glwt_x11_h

#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>

struct glwt_x11
{
    Display *display;
    XContext xcontext;
    int screen_num;
    Visual *visual;
    int depth;
    int visual_id;
    Colormap colormap;
};

struct glwt_window_x11
{
    Window window;
};

#endif
