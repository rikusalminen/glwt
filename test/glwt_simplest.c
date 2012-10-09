#include <GLWT/glwt.h>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    GLWTWindow *window = 0;
    if(glwtInit(0, 0) != 0 ||
        !(window = glwtWindowCreate("", 400, 300, 0, 0)))
        goto error;

    glwtWindowShow(window, 1);

    while(!glwtWindowClosed(window)
        && glwtEventHandle(1) == 0)
    {
    }

    glwtWindowShow(window, 0);

error:
    glwtWindowDestroy(window);
    glwtQuit();
    return 0;
}
