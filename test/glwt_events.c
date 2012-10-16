#include <stdio.h>

#include <GLWT/glwt.h>

static void error_callback(const char *msg, void *userdata)
{
    (void)userdata;
    fprintf(stderr, "%s\n", msg);
}

static void close_callback(GLWTWindow *window, void *userdata)
{
    (void)window; (void)userdata;
    printf("Window closed\n");
}

static void expose_callback(GLWTWindow *window, void *userdata)
{
    (void)window; (void)userdata;
    printf("Window exposed\n");
}

static void resize_callback(GLWTWindow *window, int width, int height, void *userdata)
{
    (void)window; (void)userdata;
    printf("Window resized  width: %d  height: %d\n", width, height);
}

static void show_callback(GLWTWindow *window, int show, void *userdata)
{
    (void)window; (void)userdata;
    printf("Window %s\n", show ? "show" : "hide");
}

static void focus_callback(GLWTWindow *window, int focus, void *userdata)
{
    (void)window; (void)userdata;
    printf("Window focus %s\n", focus ? "got" : "lost");
}

static void key_callback(GLWTWindow *window, int down, int keysym, int scancode, int mod, void *userdata)
{
    (void)window; (void)userdata;
    printf("Key %s  keysym: 0x%x  scancode: %d  mod: %X\n", down ? "down" : "up", keysym, scancode, mod);
}

static void motion_callback(GLWTWindow *window, int x, int y, int buttons, void *userdata)
{
    (void)window; (void)userdata;
    printf("Motion  x: %d  y: %d  buttons: %X\n", x, y, buttons);
}

static void button_callback(GLWTWindow *window, int down, int x, int y, int button, int mod, void *userdata)
{
    (void)window; (void)userdata;
    printf("Button %s  x: %d  y: %d  button: %d  mod: %X\n", down ? "down" : "up", x, y, button, mod);
}

static void mouseover_callback(GLWTWindow *window, int enter, void *userdata)
{
    (void)window; (void)userdata;
    printf("Mouse %s\n", enter ? "enter" : "leave");
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

    GLWTAppCallbacks app_callbacks = {
        .error_callback = error_callback,
        .userdata = 0,
    };

    if(glwtInit(&glwt_config, &app_callbacks) != 0)
        goto error;

    GLWTWindowCallbacks win_callbacks = {
        .close_callback = close_callback,
        .expose_callback = expose_callback,
        .resize_callback = resize_callback,
        .show_callback = show_callback,
        .focus_callback = focus_callback,
        .key_callback = key_callback,
        .motion_callback = motion_callback,
        .button_callback = button_callback,
        .mouseover_callback = mouseover_callback,
        .userdata = 0
    };

    GLWTWindow *window = 0;
    if(!(window = glwtWindowCreate("", 400, 300, &win_callbacks, 0)))
        goto error;

    glwtWindowShow(window, 1);
    glwtMakeCurrent(window);
    glwtSwapInterval(window, 1);

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
