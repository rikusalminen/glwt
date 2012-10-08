#include <stdio.h>

#include <GLWT/glwt.h>

static void error_callback(const char *msg, void *userdata)
{
    (void)userdata;
    fputs(msg, stderr);
}

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    GLWTConfig glwt_config = {
        .red_bits = 0,
        .green_bits = 0,
        .blue_bits = 0,
        .alpha_bits = 0,
        .depth_bits = 0,
        .stencil_bits = 0,
        .samples = 0,
        .sample_buffers = 0,
        .api = GLWT_API_ANY | GLWT_PROFILE_DEBUG,
        .api_version_major = 0,
        .api_version_minor = 0
    };

    GLWTAppCallbacks app_callbacks = {
        .error_callback = error_callback,
        .userdata = 0,
    };

    if(glwtInit(&glwt_config, &app_callbacks) != 0)
        return -1;

    glwtQuit();

    return 0;
}
