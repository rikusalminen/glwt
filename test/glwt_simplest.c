#include <GLWT/glwt.h>

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    if(glwtInit(0, 0) != 0)
        return -1;

    glwtQuit();

    return 0;
}
