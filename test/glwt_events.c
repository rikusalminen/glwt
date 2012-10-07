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

    GLWTConfig glwt_config = { };

    GLWTAppCallbacks app_callbacks = {
        .error_callback = error_callback,
        .userdata = 0,
    };

    if(glwtInit(&glwt_config, &app_callbacks) != 0)
        return -1;

    glwtQuit();

    return 0;
}
