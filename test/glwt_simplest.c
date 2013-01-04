#include <GLWT/glwt.h>

int main(int argc, char *argv[])
{
    GLWTWindow *window = 0;

    (void)argc; (void)argv;

    if(glwtInit(0, 0, 0) != 0 ||
        !(window = glwtWindowCreate("GLWT Simplest", 400, 300, 0, 0, 0)))
        goto error;

    glwtWindowShow(window, 1);
    glwtMakeCurrent(window);
    glwtSwapInterval(window, 1);

    while(!glwtWindowClosed(window)
        && glwtEventHandle(1) == 0)
    {
        glwtSwapBuffers(window);
    }

    glwtWindowShow(window, 0);

error:
    glwtWindowDestroy(window);
    glwtQuit();
    return 0;
}
