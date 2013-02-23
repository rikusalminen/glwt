#ifndef GLWT_glwt_internal_h
#define GLWT_glwt_internal_h

#include <GLWT/glwt.h>

#if defined(WIN32)
    #include <win32/glwt_win32.h>

    #ifndef GLWT_USE_EGL
        #include <wgl/glwt_wgl.h>
    #endif
#elif defined(__APPLE__)
    #include <osx/glwt_osx.h>
#elif defined(RASPBERRYPI)
    #include <rpi/glwt_rpi.h>
#elif defined(ANDROID)
    #ifndef GLWT_USE_EGL
    #define GLWT_USE_EGL
    #endif

    #include <android/glwt_android.h>
#else
    #include <x11/glwt_x11.h>

    #ifndef GLWT_USE_EGL
        #include <glx/glwt_glx.h>
    #endif
#endif

#ifdef GLWT_USE_EGL
    #include <egl/glwt_egl.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct glwt
{
    void (*error_callback)(const char *msg, void *userdata);
    void *userdata;

    int api;
    int api_version_major, api_version_minor;

#if defined(WIN32)
    struct glwt_win32 win32;
    #ifndef GLWT_USE_EGL
    struct glwt_wgl wgl;
    #endif
#elif defined(__APPLE__)
    struct glwt_osx osx;
#elif defined(RASPBERRYPI)
    struct glwt_rpi rpi;
#elif defined(ANDROID)
    struct glwt_android android;
#else
    struct glwt_x11 x11;
    #ifndef GLWT_USE_EGL
    struct glwt_glx glx;
    #endif
#endif
#ifdef GLWT_USE_EGL
    struct glwt_egl egl;
#endif
};

extern struct glwt glwt;

struct GLWTWindow
{
    void (*win_callback)(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata);
    void *userdata;
    int closed;

#if defined(WIN32)
    struct glwt_window_win32 win32;
    #ifndef GLWT_USE_EGL
    struct glwt_window_wgl wgl;
    #endif
#elif defined(__APPLE__)
    struct glwt_window_osx osx;
#elif defined(RASPBERRYPI)
    struct glwt_window_rpi rpi;
#elif defined(ANDROID)
    struct glwt_window_android android;
#else
    struct glwt_window_x11 x11;
    #ifndef GLWT_USE_EGL
    struct glwt_window_glx glx;
    #endif
#endif
#ifdef GLWT_USE_EGL
    struct glwt_window_egl egl;
#endif
};

int glwtErrorPrintf(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
