#include <GLWT/glwt.h>

#ifdef GLWT_TESTS_GLES
#include <GLXW/glxw_es2.h>
#else
#include <GLXW/glxw.h>
#endif

int main(int argc, char *argv[])
{
    GLWTWindow *window = 0;

    (void)argc; (void)argv;

    if(glwtInit(0, 0, 0) != 0 ||
        !(window = glwtWindowCreate("GLWT Simplest", 400, 300, 0, 0)))
        goto error;

    glwtWindowShow(window, 1);
    glwtMakeCurrent(window);
    glwtSwapInterval(window, 1);

    glxwInit();

    while(!glwtWindowClosed(window)
        && glwtEventHandle(1) == 0)
    {
        glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glwtSwapBuffers(window);
    }

    glwtWindowShow(window, 0);
    glwtMakeCurrent(0);

error:
    glwtWindowDestroy(window);
    glwtQuit();
    return 0;
}
