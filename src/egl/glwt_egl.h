#ifndef GLWT_glwt_egl_h
#define GLWT_glwt_egl_h

#include <EGL/egl.h>

#include <GLXW/glxw_egl.h>

struct glwt_egl
{
    EGLDisplay display;
    int version_major, version_minor;
    EGLConfig config;
    int egl_api;
};

struct glwt_window_egl
{
    EGLContext context;
    EGLSurface surface;
};

int glwtInitEGL(const GLWTConfig *config);
void glwtQuitEGL();

int glwtWindowCreateEGL(GLWTWindow *win, GLWTWindow *share);
void glwtWindowDestroyEGL(GLWTWindow *win);

#endif
