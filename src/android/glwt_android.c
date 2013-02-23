#include <android_native_app_glue.h>

#include <glwt_internal.h>

int32_t glwt_input_event_callback(struct android_app* android_app, AInputEvent* event);
void glwt_app_cmd_callback(struct android_app *android_app, int32_t cmd);


void glwtAndroidApp(struct android_app *android_app)
{
    glwt.android.android_app = android_app;
}

int glwtInit(
    const GLWTConfig *config,
    void (*error_callback)(const char *msg, void *userdata),
    void *userdata
    )
{
    glwt.error_callback = error_callback;
    glwt.userdata = userdata;

    glwt.android.android_app->userData = NULL;
    glwt.android.android_app->onAppCmd = glwt_app_cmd_callback;
    glwt.android.android_app->onInputEvent = glwt_input_event_callback;

    if(glwtInitEGL(config) != 0)
        goto error;

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
