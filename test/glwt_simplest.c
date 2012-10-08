#include <GLWT/glwt.h>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    GLWTWindow *window = 0;
    if(glwtInit(0, 0) != 0 ||
        !(window = glwtWindowCreate("", 400, 300, 0, 0)))
        goto error;

error:
    glwtWindowDestroy(window);
    glwtQuit();
    return 0;
}
