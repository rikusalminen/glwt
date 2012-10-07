#ifndef GLWT_glwt_x11_h
#define GLWT_glwt_x11_h

#include <X11/Xlib.h>
#include <X11/Xutil.h>

struct glwt_x11
{
    Display *display;
    int screen_num;
    XVisualInfo visual_info;
    int visual_id;
    Colormap colormap;
};

#endif
