#include <stdarg.h>
#include <stdio.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

struct glwt glwt;

int glwtErrorPrintf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int size = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    char buffer[size+1];
    va_start(ap, fmt);
    int val = vsnprintf(buffer, size+1, fmt, ap);
    va_end(ap);

    if(glwt.error_callback)
        glwt.error_callback(buffer, glwt.userdata);
    else
        fprintf(stderr, "%s\n", buffer);

    return val;
}

int glwtWindowClosed(GLWTWindow *window)
{
    return window->closed;
}
