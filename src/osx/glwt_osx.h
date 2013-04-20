#ifndef GLWT_glwt_osx_h
#define GLWT_glwt_osx_h

#if defined(__OBJC__)
#import <Cocoa/Cocoa.h>
#else
#include <ApplicationServices/ApplicationServices.h>
typedef void* id;
#endif

struct glwt_osx
{
    CGDisplayModeRef desktop_mode;
    id app;
    id autorelease_pool;
    id pixel_format;
};

struct glwt_window_osx
{
    id nswindow;
    id ctx;
    id view;
    id delegate;

    unsigned int modifier_flags;
};

#endif
