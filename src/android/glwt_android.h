#ifndef GLWT_glwt_android_h
#define GLWT_glwt_android_h

#include <android/native_activity.h>

struct glwt_android
{
    ANativeActivity *activity;
    GLWTWindow *window;
};

struct glwt_window_android
{
    ANativeWindow *native_window;
};

#endif
