#include <stdio.h>

#include <GLWT/glwt.h>

static void error_callback(const char *msg, void *userdata)
{
    (void)userdata;
    fprintf(stderr, "%s\n", msg);
}

static void window_callback(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata)
{
    (void)window;
    (void)userdata;

    switch(event->type)
    {
        case GLWT_WINDOW_CLOSE:
            printf("Window closed\n");
            break;
        case GLWT_WINDOW_EXPOSE:
            printf("Window exposed\n");
            break;
        case GLWT_WINDOW_RESIZE:
            printf("Window resized  width: %d  height: %d\n", event->resize.width, event->resize.height);
            break;
        case GLWT_WINDOW_SHOW:
        case GLWT_WINDOW_HIDE:
            printf("Window %s\n", (event->type == GLWT_WINDOW_SHOW) ? "show" : "hide");
            break;
        case GLWT_WINDOW_FOCUS_IN:
        case GLWT_WINDOW_FOCUS_OUT:
            printf("Window focus %s\n", (event->type == GLWT_WINDOW_FOCUS_IN) ? "in" : "out");
            break;
        case GLWT_WINDOW_KEY_UP:
        case GLWT_WINDOW_KEY_DOWN:
            printf("Key %s  keysym: 0x%x  scancode: %d  mod: %X\n",
                (event->type == GLWT_WINDOW_KEY_DOWN) ? "down" : "up",
                event->key.keysym, event->key.scancode, event->key.mod);
            break;
        case GLWT_WINDOW_BUTTON_UP:
        case GLWT_WINDOW_BUTTON_DOWN:
            printf("Button %s  x: %d  y: %d  button: %d  mod: %X\n",
                (event->type == GLWT_WINDOW_BUTTON_DOWN) ? "down" : "up",
                event->button.x, event->button.y, event->button.button, event->button.mod);
            break;
        case GLWT_WINDOW_MOUSE_MOTION:
            printf("Motion  x: %d  y: %d  buttons: %X\n",
                event->motion.x, event->motion.y, event->motion.buttons);
            break;
        case GLWT_WINDOW_MOUSE_ENTER:
        case GLWT_WINDOW_MOUSE_LEAVE:
            printf("Mouse %s\n", (event->type == GLWT_WINDOW_MOUSE_ENTER) ? "enter" : "leave");
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;
    int err = -1;

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

    if(glwtInit(&glwt_config, error_callback, NULL) != 0)
        goto error;

    GLWTWindow *window = 0;
    if(!(window = glwtWindowCreate("", 400, 300, NULL, window_callback, NULL)))
        goto error;

    glwtWindowShow(window, 1);
    glwtMakeCurrent(window);
    glwtSwapInterval(window, 1);

    int width, height;
    glwtWindowGetSize(window, &width, &height);
    printf("Window size: %d x %d\n", width, height);

    while(!glwtWindowClosed(window))
    {
        if(glwtEventHandle(1) != 0)
            goto error;

        glwtSwapBuffers(window);
    }

    err = 0;
error:
    glwtWindowDestroy(window);
    glwtQuit();
    return err;
}
