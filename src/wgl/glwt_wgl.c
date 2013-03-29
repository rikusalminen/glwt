#include <GLWT/glwt.h>
#include <glwt_internal.h>

int glwtInitWGL(const GLWTConfig *config)
{
    UINT num_formats;
    int pixel_attribs[] = {
        WGL_RED_BITS_ARB, config ? config->red_bits : 0,
        WGL_GREEN_BITS_ARB, config ? config->green_bits : 0,
        WGL_BLUE_BITS_ARB, config ? config->blue_bits : 0,
        WGL_ALPHA_BITS_ARB, config ? config->alpha_bits : 0,
        WGL_DEPTH_BITS_ARB, config ? config->depth_bits : 0,
        WGL_STENCIL_BITS_ARB, config ? config->stencil_bits : 0,
        WGL_SAMPLES_ARB, config ? config->samples : 0,
        WGL_SAMPLE_BUFFERS_ARB, config ? config->sample_buffers : 0,
        WGL_SUPPORT_OPENGL_ARB, 1,
        WGL_DOUBLE_BUFFER_ARB, 1,
        WGL_DRAW_TO_WINDOW_ARB, 1,
        0, 0
    };

    HGLRC old_context = 0, temp_context = 0;
    HDC old_hdc = 0;

    glwt.api = config ? config->api : GLWT_API_ANY;
    glwt.api_version_major = config ? config->api_version_major : 0;
    glwt.api_version_minor = config ? config->api_version_minor : 0;

    old_context = wglGetCurrentContext();
    old_hdc = wglGetCurrentDC();

    temp_context = wglCreateContext(glwt.win32.dummy_hdc);
    if(!temp_context)
    {
        glwtWin32Error("wglCreateContext failed");
        goto error;
    }

    if(!wglMakeCurrent(glwt.win32.dummy_hdc, temp_context))
    {
        glwtWin32Error("wglMakeCurrent failed");
        goto error;
    }

    if(glxwInitWGL() != 0)
        goto error;

    // Required: WGL_ARB_extensions_string, WGL_ARB_pixel_format, WGL_ARB_multisample, WGL_ARB_make_current_read, WGL_EXT_swap_control

    if(!wglChoosePixelFormatARB(glwt.win32.dummy_hdc, pixel_attribs, NULL, 1, &glwt.wgl.pixel_format, &num_formats) ||
        num_formats == 0)
    {
        glwtWin32Error("wglChoosePixelFormatARB failed");
        goto error;
    }

    wglMakeCurrent(old_hdc, old_context);
    wglDeleteContext(temp_context);

    return 0;
error:

    wglMakeCurrent(old_hdc, old_context);
    if(temp_context)
        wglDeleteContext(temp_context);

    glwtQuitWGL();
    return -1;
}

void glwtQuitWGL()
{
}
