#include <GLWT/glwt.h>
#include <glwt_internal.h>
#include <termios.h>

static void set_echo(int on)
{
    struct termios newtermios;
    tcgetattr(STDIN_FILENO, &newtermios);

    if(on)
        newtermios.c_lflag |= ECHO;
    else
        newtermios.c_lflag &= ~ECHO;

    tcsetattr(STDIN_FILENO, TCSANOW, &newtermios);
}

int glwtInit(
    const GLWTConfig *config,
    void (*event_callback)(const GLWTEvent *event),
    void (*error_callback)(const char *msg))
{
    glwt.event_callback = event_callback;
    glwt.error_callback = error_callback;

    bcm_host_init();

    if(glwtInitEGL(config) != 0)
        goto error;
    glwt.rpi.dispman_display = vc_dispmanx_display_open(0);

    if(0 > graphics_get_display_size(0, &glwt.rpi.width, &glwt.rpi.height))
        goto error;

    set_echo(0);

    return 0;
error:
    glwtQuit();
    return -1;
}

void glwtQuit()
{
    set_echo(1);

    glwtQuitEGL();

    vc_dispmanx_display_close(glwt.rpi.dispman_display);

    bcm_host_deinit();

    memset(&glwt, 0, sizeof(struct glwt));
}
