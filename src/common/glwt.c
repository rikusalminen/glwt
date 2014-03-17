#define _CRT_SECURE_NO_WARNINGS
#include <stdarg.h>
#include <stdio.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

#ifdef _WIN32
#include <malloc.h>
#ifndef alloca
#define alloca _alloca
#endif
#else
#include <alloca.h>
#endif

struct glwt glwt;

int glwtErrorPrintf(const char *fmt, ...)
{
    char *buffer;
    int size, val;

    va_list ap;
    va_start(ap, fmt);
    size = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    buffer = (char *)alloca(size+1);
    va_start(ap, fmt);
    val = vsnprintf(buffer, size+1, fmt, ap);
    va_end(ap);

    if(glwt.error_callback)
        glwt.error_callback(buffer);
    else
        fprintf(stderr, "%s\n", buffer);

    return val;
}

int glwtWindowClosed(GLWTWindow *window)
{
    return window->closed;
}

void *glwtWindowGetPtr(GLWTWindow *window)
{
    return window->userdata;
}
