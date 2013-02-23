#ifndef GLWT_glwt_android_h
#define GLWT_glwt_android_h

#include <android_native_app_glue.h>

struct glwt_android
{
    struct android_app *android_app;
    GLWTWindow *window;
};

struct glwt_window_android
{
};

#endif
