#include <string.h>

#include <glwt_internal.h>

int glwtInit(
    const GLWTConfig *config,
    void (*error_callback)(const char *msg, void *userdata),
    void *userdata
    )
{
    glwt.error_callback = error_callback;
    glwt.userdata = userdata;

    if(glwtInitEGL(config) != 0)
        goto error;

    glwtEventHandle(0); // no-op, don't strip ANativeActivity_onCreate

    return 0;
error:
    glwtQuit();
    return -1;
}

void glwtQuit()
{
    glwtQuitEGL();

    memset(&glwt, 0, sizeof(struct glwt));
}

void glwtAppTerminate()
{
    ANativeActivity_finish(glwt.android.activity);
}
