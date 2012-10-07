#ifndef GLWT_glwt_glx_h
#define GLWT_glwt_glx_h

#include <GLXW/glxw_glx.h>

struct glwt_glx
{
    int version_major, version_minor;
    GLXFBConfig fbconfig;
};

int glwtInitGLX(const GLWTConfig *config);
void glwtQuitGLX();

#endif
