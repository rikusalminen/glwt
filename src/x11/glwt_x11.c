#include <X11/Xlib.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

int glwtInit(const GLWTConfig *config, const GLWTAppCallbacks *app_callbacks)
{
    if(app_callbacks)
        glwt.app_callbacks = *app_callbacks;

    XInitThreads();

    glwtErrorPrintf("Xlib sucks!\n");
    return -1;
}

void glwtQuit()
{
}
