#ifndef GLWT_glwt_wgl_h
#define GLWT_glwt_wgl_h

#include <GLXW/glxw_wgl.h>

struct glwt_wgl
{
    int dummy;
};

struct glwt_window_wgl
{
    HGLRC context;
};

int glwtInitWGL(const GLWTConfig *config);
void glwtQuitWGL();

int glwtWindowCreateWGL(GLWTWindow *win, GLWTWindow *share);
void glwtWindowDestroyWGL(GLWTWindow *window);

#endif
