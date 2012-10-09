#include <GL/glx.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

int glwtInitGLX(const GLWTConfig *config)
{
    if(config && (config->api & GLWT_API_MASK) && !(config->api & GLWT_API_OPENGL))
    {
        glwtErrorPrintf("GLX can only initialize OpenGL profiles");
        return -1;
    }

    glwt.api = config ? config->api : 0;
    glwt.api_version_major = config ? config->api_version_major : 0;
    glwt.api_version_minor = config ? config->api_version_minor : 0;

    if(glxwInitGLX() != 0)
    {
        glwtErrorPrintf("GLX Initialization failed");
        return -1;
    }

    glXQueryVersion(glwt.x11.display, &glwt.glx.version_major, &glwt.glx.version_minor);
    // Required: GLX version 1.4, GL_ARB_create_context, GL_ARB_create_context_profile

    const int fbconfig_attribs[] = {
        GLX_DOUBLEBUFFER, 1,
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_RED_SIZE, config ? config->red_bits : 0,
        GLX_GREEN_SIZE, config ? config->green_bits : 0,
        GLX_BLUE_SIZE, config ? config->blue_bits : 0,
        GLX_ALPHA_SIZE, config ? config->alpha_bits : 0,
        GLX_DEPTH_SIZE, config ? config->depth_bits : 0,
        GLX_STENCIL_SIZE, config ? config->stencil_bits : 0,
        GLX_SAMPLES, config ? config->samples : 0,
        GLX_SAMPLE_BUFFERS, config ? config->sample_buffers : 0,
        0, 0,
    };

    int num_fbconfigs = 0;
    GLXFBConfig *fbconfigs = glXChooseFBConfig(
        glwt.x11.display,
        glwt.x11.screen_num,
        fbconfig_attribs,
        &num_fbconfigs);

    if(!fbconfigs)
    {
        glwtErrorPrintf("glXChooseFBConfig failed");
        return -1;
    }

    if(num_fbconfigs == 0)
    {
        XFree(fbconfigs);
        glwtErrorPrintf("No suitable fb configs found");
        return -1;
    }

    glwt.glx.fbconfig = fbconfigs[0];
    XFree(fbconfigs);

    XVisualInfo *vi = glXGetVisualFromFBConfig(glwt.x11.display, glwt.glx.fbconfig);
    if(!vi)
    {
        glwtErrorPrintf("glXGetVisualFromFBConfig failed");
        return -1;
    }

    glwt.x11.visual = vi->visual;
    glwt.x11.depth = vi->depth;
    XFree(vi);

    glXGetFBConfigAttrib(glwt.x11.display, glwt.glx.fbconfig, GLX_VISUAL_ID, &glwt.x11.visual_id);

    return 0;
}

void glwtQuitGLX()
{
}
