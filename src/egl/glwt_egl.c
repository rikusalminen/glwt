#include <GLWT/glwt.h>

#include <glwt_internal.h>

int glwtInitEGL(const GLWTConfig *config)
{
    if(glxwInitEGL() != 0)
    {
        glwtErrorPrintf("Initializing EGL extensions failed");
        return -1;
    }

    glwt.api = config ? config->api : GLWT_API_OPENGL_ES;
    glwt.api_version_major = config ? config->api_version_major : 2;
    glwt.api_version_minor = config ? config->api_version_minor : 0;

    if(!glwt.egl.display)
    {
        glwt.egl.display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if(!glwt.egl.display)
        {
            glwtErrorPrintf("eglGetDisplay failed");
            goto error;
        }
    }

    if(!eglInitialize(glwt.egl.display, &glwt.egl.version_major, &glwt.egl.version_minor))
    {
        glwtErrorPrintf("EGL initialization failed");
        return -1;
    }

    int egl_api_bits = 0;

    if(!config ||
        (config->api & GLWT_API_MASK) == GLWT_API_ANY ||
        ((config->api & GLWT_API_MASK) == GLWT_API_OPENGL_ES &&
            (config->api_version_major == 0 || config->api_version_major == 2)))
        egl_api_bits = EGL_OPENGL_ES2_BIT;
    else if(config && (config->api & GLWT_API_MASK) == GLWT_API_OPENGL_ES && config->api_version_major == 1)
        egl_api_bits = EGL_OPENGL_ES_BIT;
    else if(config && (config->api & GLWT_API_MASK) == GLWT_API_OPENGL)
        egl_api_bits = EGL_OPENGL_BIT;

    int config_attribs[] = {
        EGL_RED_SIZE, config ? config->red_bits : 0,
        EGL_GREEN_SIZE, config ? config->green_bits : 0,
        EGL_BLUE_SIZE, config ? config->blue_bits : 0,
        EGL_ALPHA_SIZE, config ? config->alpha_bits : 0,
        EGL_DEPTH_SIZE, config ? config->depth_bits : 0,
        EGL_STENCIL_SIZE, config ? config->stencil_bits : 0,
        EGL_SAMPLES, config ? config->samples : 0,
        EGL_SAMPLE_BUFFERS, config ? config->sample_buffers : 0,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, egl_api_bits,
        EGL_NONE
    };

    int num_configs;
    if(!eglChooseConfig(glwt.egl.display, config_attribs, &glwt.egl.config, 1, &num_configs) ||
        num_configs != 1)
    {
        glwtErrorPrintf("eglChooseConfig failed");
        goto error;
    }

#ifdef GLWT_glwt_x11_h
    if(!eglGetConfigAttrib(glwt.egl.display, glwt.egl.config, EGL_NATIVE_VISUAL_ID, &glwt.x11.visual_id))
    {
        glwtErrorPrintf("eglGetConfigAttrib failed");
        goto error;
    }
#endif

    return 0;
error:
    glwtQuitEGL();
    return -1;
}

void glwtQuitEGL()
{
    eglTerminate(glwt.egl.display);
}
