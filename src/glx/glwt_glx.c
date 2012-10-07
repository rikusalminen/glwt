#include <GL/glx.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

int glwtInitGLX(const GLWTConfig *config)
{
    if(glxwInitGLX() != 0)
    {
        glwtErrorPrintf("GLX Initialization failed");
        return -1;
    }

    glXQueryVersion(glwt.x11.display, &glwt.glx.version_major, &glwt.glx.version_minor);

    const int config_attribs[] = {
        0,
    };

    int num_configs = 0;
    GLXFBConfig *configs = glXChooseFBConfig(
        glwt.x11.display,
        glwt.x11.screen_num,
        config_attribs,
        &num_configs);

    if(!configs)
    {
        glwtErrorPrintf("glXChooseFBConfig failed");
        return -1;
    }

    if(num_configs == 0)
    {
        XFree(configs);
        glwtErrorPrintf("No suitable fb configs found");
        return -1;
    }

    glwt.glx.fbconfig = configs[0];
    XFree(configs);

    XVisualInfo *vi = glXGetVisualFromFBConfig(glwt.x11.display, glwt.glx.fbconfig);
    if(!vi)
    {
        glwtErrorPrintf("glXGetVisualFromFBConfig failed");
        return -1;
    }
    glwt.x11.visual_info = *vi;
    XFree(vi);

    glXGetFBConfigAttrib(glwt.x11.display, glwt.glx.fbconfig, GLX_VISUAL_ID, &glwt.x11.visual_id);

    return 0;
}

void glwtQuitGLX()
{
}
