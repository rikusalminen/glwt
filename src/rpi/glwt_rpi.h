#ifndef GLWT_glwt_rpi_h
#define GLWT_glwt_rpi_h

#include <bcm_host.h>
#include <EGL/egl.h>

struct glwt_rpi
{
    DISPMANX_DISPLAY_HANDLE_T dispman_display;
    uint32_t width, height;
};

struct glwt_window_rpi
{
    EGL_DISPMANX_WINDOW_T nativewindow;
};

#endif
