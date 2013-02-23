#include <glwt_internal.h>

#include <android/input.h>

void glwt_app_cmd_callback(struct android_app *android_app, int32_t cmd)
{
    (void)android_app;

    switch(cmd)
    {
        case APP_CMD_INIT_WINDOW:
            if(!glwt.android.window->egl.context)
            {
                glwtWindowCreateEGL(glwt.android.window, NULL);
            }

            // show event?
            // expose event
            break;
        case APP_CMD_TERM_WINDOW:
            glwt.android.window->closed = 1;
            // close callback
            if(glwt.android.window->egl.context)
            {
                glwtWindowDestroy(glwt.android.window);
            }
            break;
        case APP_CMD_WINDOW_REDRAW_NEEDED:
            // expose event
            break;

        case APP_CMD_INPUT_CHANGED:
            break;
        case APP_CMD_WINDOW_RESIZED:
            break;
        case APP_CMD_CONTENT_RECT_CHANGED:
            break;
        case APP_CMD_GAINED_FOCUS:
            break;
        case APP_CMD_LOST_FOCUS:
            break;
        case APP_CMD_CONFIG_CHANGED:
            break;
        case APP_CMD_LOW_MEMORY:
            break;
        case APP_CMD_START:
            break;
        case APP_CMD_RESUME:
            break;
        case APP_CMD_SAVE_STATE:
            android_app->savedState = NULL;
            android_app->savedStateSize = 0;
            break;
        case APP_CMD_PAUSE:
            break;
        case APP_CMD_STOP:
            break;
        case APP_CMD_DESTROY:
            break;
        default:
            break;
    }
}

int32_t glwt_input_event_callback(struct android_app* android_app, AInputEvent* event)
{
    (void)android_app;
    (void)event;

    switch(AInputEvent_getType(event))
    {
        case AINPUT_EVENT_TYPE_KEY:
        case AINPUT_EVENT_TYPE_MOTION:
            //return 1;
        default:
            break;
    }

    return 0;
}

int glwtEventHandle(int wait)
{
    int events = 0, ident = 0;
    struct android_poll_source *source;

    while(!glwt.android.android_app->destroyRequested &&
        (ident = ALooper_pollAll(wait ? -1 : 0, NULL, &events, (void**)&source)) >= 0)
    {
        wait = 0;

        if(source)
            source->process(glwt.android.android_app, source);
    }

    return 0;
}
