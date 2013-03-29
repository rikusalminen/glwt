#ifndef GLWT_glwt_glx_h
#define GLWT_glwt_glx_h

#include <GLXW/glxw_glx.h>

struct glwt_glx
{
    GLXFBConfig fbconfig;
    int visual_id;
};

struct glwt_window_glx
{
    GLXWindow surface;
    GLXContext context;
};

int glwtInitGLX(const GLWTConfig *config);
void glwtQuitGLX();

int glwtWindowCreateGLX(GLWTWindow *win, GLWTWindow *share);
void glwtWindowDestroyGLX(GLWTWindow *win);

#endif
