#ifndef GLWT_glwt_x11_h
#define GLWT_glwt_x11_h

#include <X11/Xlib.h>

struct glwt_x11
{
    Display *display;
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
