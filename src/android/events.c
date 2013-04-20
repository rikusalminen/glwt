#include <glwt_internal.h>

#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, __FILE__, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, __FILE__, __VA_ARGS__))

int glwtEventHandle(int wait)
{
    (void)wait;
    return 0;
}

static void handle_event_key(AInputEvent *event)
{
    (void)event;

    LOGI("***** handle event key *****\n");
}

static void handle_event_motion(AInputEvent *event)
{
    int32_t source = AInputEvent_getSource(event);

    size_t pointer_count = AMotionEvent_getPointerCount(event);

    size_t pointer_index = 0;
    float x = AMotionEvent_getX(event, pointer_index);
    float y = AMotionEvent_getY(event, pointer_index);

    LOGI("***** handle event motion %x  %u:  %f %f*****\n", source, pointer_count, x, y);

    /*
    AINPUT_SOURCE_KEYBOARD
    AINPUT_SOURCE_DPAD
    AINPUT_SOURCE_TOUCHSCREEN
    AINPUT_SOURCE_MOUSE
    AINPUT_SOURCE_TRACKBALL
    AINPUT_SOURCE_TOUCHPAD
       --- it seems like there are more input sources that are missing from NDK headers
    */
}

static void handle_event(AInputEvent *event)
{
    switch(AInputEvent_getType(event))
    {
        case AINPUT_EVENT_TYPE_KEY:
            handle_event_key(event);
            return;
        case AINPUT_EVENT_TYPE_MOTION:
            handle_event_motion(event);
            return;
        default:
            break;
    }
}

static int input_callback(int fd, int events, void* data)
{
    (void)fd;
    (void)events;
    AInputQueue *input_queue = (AInputQueue*)data;

    int32_t has_events = AInputQueue_hasEvents(input_queue);
    if(has_events < 0)
        LOGW("**** AInputQueue_hasEvents FAILED");

    if(has_events > 0)
    {
        AInputEvent* event;
        if(AInputQueue_getEvent(input_queue, &event) < 0)
            LOGW("**** AInputQueue_getEvent FAILED");
        else
        {
            if(AInputQueue_preDispatchEvent(input_queue, event) == 0)
            {
                int handled = 0;
                handle_event(event);
                AInputQueue_finishEvent(input_queue, event, handled);
            }
        }
    }

    return 1;
}

static void onStart(ANativeActivity *activity)
{
    (void)activity;
    LOGI("ANativeActivity onStart");
}

static void onResume(ANativeActivity *activity)
{
    (void)activity;
    LOGI("ANativeActivity onResume");
}

static void* onSaveInstanceState(ANativeActivity *activity, size_t *outSize)
{
    (void)activity;
    LOGI("ANativeActivity onSaveInstanceState");

    *outSize = 0;
    return 0;
}

static void onPause(ANativeActivity *activity)
{
    (void)activity;
    LOGI("ANativeActivity onPause");
}

static void onStop(ANativeActivity *activity)
{
    (void)activity;
    LOGI("ANativeActivity onStop");
}

static void onDestroy(ANativeActivity *activity)
{
    (void)activity;
    LOGI("ANativeActivity onDestroy");

    glwtWindowDestroy(glwt.android.window);
    glwtQuit();
}

static void onWindowFocusChanged(ANativeActivity *activity, int hasFocus)
{
    (void)activity;
    (void)hasFocus;
    LOGI("ANativeActivity onWindowFocusChanged");

    if(glwt.android.window->win_callback)
    {
        GLWTWindowEvent event;
        event.type = hasFocus ? GLWT_WINDOW_FOCUS_IN : GLWT_WINDOW_FOCUS_OUT;
        event.window = glwt.android.window;
        glwt.android.window->win_callback(glwt.android.window, &event, glwt.android.window->userdata);
    }
}

static void onNativeWindowCreated(ANativeActivity *activity, ANativeWindow *native_window)
{
    (void)activity;
    LOGI("ANativeActivity onNativeWindowCreated");

    if(ANativeWindow_setBuffersGeometry(native_window, 0, 0, glwt.egl.visual_id) < 0)
    {
        glwtErrorPrintf("ANativeWindow_setBuffersGeometry failed\n");
        return;
    }

    if(glwtWindowCreateSurfaceEGL(glwt.android.window, native_window) == 0)
        glwt.android.window->android.native_window = native_window;

    if(glwt.android.window->win_callback)
    {
        GLWTWindowEvent event;
        event.type = GLWT_WINDOW_SURFACE_CREATE;
        event.window = glwt.android.window;
        glwt.android.window->win_callback(glwt.android.window, &event, glwt.android.window->userdata);
    }
}

static void onNativeWindowResized(ANativeActivity *activity, ANativeWindow *native_window)
{
    (void)activity;
    LOGI("ANativeActivity onNativeWindowResized");

    if(glwt.android.window->win_callback)
    {
        GLWTWindowEvent event;
        event.type = GLWT_WINDOW_RESIZE;
        event.window = glwt.android.window;
        event.resize.width = ANativeWindow_getWidth(native_window);
        event.resize.height = ANativeWindow_getHeight(native_window);
        glwt.android.window->win_callback(glwt.android.window, &event, glwt.android.window->userdata);
    }
}

static void onNativeWindowRedrawNeeded(ANativeActivity *activity, ANativeWindow *native_window)
{
    (void)activity;
    (void)native_window;

    LOGI("ANativeActivity onNativeWindowRedrawNeeded");

    if(glwt.android.window->win_callback)
    {
        GLWTWindowEvent event;
        event.type = GLWT_WINDOW_EXPOSE;
        event.window = glwt.android.window;
        glwt.android.window->win_callback(glwt.android.window, &event, glwt.android.window->userdata);
    }
}

static void onNativeWindowDestroyed(ANativeActivity *activity, ANativeWindow *native_window)
{
    (void)activity;
    (void)native_window;
    LOGI("ANativeActivity onNativeWindowDestroyed");

    if(glwt.android.window->win_callback)
    {
        GLWTWindowEvent event;
        event.type = GLWT_WINDOW_SURFACE_DESTROY;
        event.window = glwt.android.window;
        glwt.android.window->win_callback(glwt.android.window, &event, glwt.android.window->userdata);
    }

    glwtWindowDestroySurfaceEGL(glwt.android.window);
    glwt.android.window->android.native_window = 0;
}

static void onInputQueueCreated(ANativeActivity *activity, AInputQueue *input_queue)
{
    (void)activity;

    ALooper* looper = ALooper_forThread();
    int ident = ALOOPER_POLL_CALLBACK;
    int (*callback)(int fd, int events, void* data) = input_callback;
    void *data = (void*)input_queue;
    AInputQueue_attachLooper(input_queue, looper, ident, callback, data);
}

static void onInputQueueDestroyed(ANativeActivity *activity, AInputQueue *input_queue)
{
    (void)activity;

    AInputQueue_detachLooper(input_queue);
}

static void onContentRectChanged(ANativeActivity *activity, const ARect* rect)
{
    (void)activity;
    (void)rect;
    LOGI("ANativeActivity onContentRectChanged %d, %d  - %d, %d\n", rect->left, rect->top, rect->right, rect->bottom);
}

static void onConfigurationChanged(ANativeActivity *activity)
{
    (void)activity;
    LOGI("ANativeActivity onConfigurationChanged");
}

static void onLowMemory(ANativeActivity *activity)
{
    (void)activity;
    LOGI("ANativeActivity onLowMemory");
}

extern GLWTWindow *glwtAppInit();

void ANativeActivity_onCreate(
    ANativeActivity *activity,
    void* saved_state,
    size_t saved_state_size)
{
    (void)saved_state;
    (void)saved_state_size;

    glwt.android.activity = activity;
    activity->instance = NULL;

    activity->callbacks->onStart = onStart;
    activity->callbacks->onResume = onResume;
    activity->callbacks->onSaveInstanceState = onSaveInstanceState;
    activity->callbacks->onPause = onPause;
    activity->callbacks->onStop = onStop;
    activity->callbacks->onDestroy = onDestroy;
    activity->callbacks->onWindowFocusChanged = onWindowFocusChanged;
    activity->callbacks->onNativeWindowCreated = onNativeWindowCreated;
    activity->callbacks->onNativeWindowResized = onNativeWindowResized;
    activity->callbacks->onNativeWindowRedrawNeeded = onNativeWindowRedrawNeeded;
    activity->callbacks->onNativeWindowDestroyed = onNativeWindowDestroyed;
    activity->callbacks->onInputQueueCreated = onInputQueueCreated;
    activity->callbacks->onInputQueueDestroyed = onInputQueueDestroyed;
    activity->callbacks->onContentRectChanged = onContentRectChanged;
    activity->callbacks->onConfigurationChanged = onConfigurationChanged;
    activity->callbacks->onLowMemory = onLowMemory;

    char *argv[] = { "GLWT", 0 };
    int argc = 1;

    GLWTWindow *window = glwtAppInit(argc, argv);

    if(!window)
        ANativeActivity_finish(activity);

    glwt.android.window = window;
}
