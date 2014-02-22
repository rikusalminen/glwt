#include <stdlib.h>
#include <string.h>

#include <GLWT/glwt.h>
#include <glwt_internal.h>

static int xlib_io_error(Display *display)
{
    (void)display;
    glwtErrorPrintf("Xlib I/O Error");
    exit(EXIT_FAILURE);
    return 0;
}

static int xlib_error(Display *display, XErrorEvent *event)
{
    (void)display;

    int buffer_size = 128;
    char buffer[buffer_size];
    XGetErrorText(display, event->error_code, buffer, buffer_size);

    glwtErrorPrintf("Xlib error code: %d  major: %d  minor: %d  message: %s",
        event->error_code,
        event->request_code, event->minor_code,
        buffer);
    return 0;
}

static int init_x11_atoms()
{
    char *atom_names[] = {
#define GLWT_X11_ATOM_NAME(X) #X,
        GLWT_X11_ATOMS(GLWT_X11_ATOM_NAME)
#undef GLWT_X11_ATOM_NAME
    };

    int num_atoms = sizeof(atom_names)/sizeof(*atom_names);

    if(XInternAtoms(glwt.x11.display, atom_names, num_atoms, False, (Atom*)&glwt.x11.atoms) == 0)
    {
        glwtErrorPrintf("XInternAtoms failed");
        return -1;
    }

    return 0;
}

int glwtInit(
    const GLWTConfig *config,
    void (*error_callback)(const char *msg, void *userdata),
    void *userdata)
{
    glwt.error_callback = error_callback;
    glwt.userdata = userdata;

    XInitThreads();

    XSetIOErrorHandler(xlib_io_error);
    XSetErrorHandler(xlib_error);

    glwt.x11.xcontext = XUniqueContext();

    if((glwt.x11.display = XOpenDisplay(NULL)) == NULL)
    {
        glwtErrorPrintf("XOpenDisplay failed");
        return -1;
    }

    glwt.x11.screen_num = DefaultScreen(glwt.x11.display);

    int xkb_major = XkbMajorVersion, xkb_minor = XkbMinorVersion;
    int xkb_opcode = -1, xkb_event = -1, xkb_error = -1;
    if(!XkbQueryExtension(glwt.x11.display, &xkb_opcode, &xkb_event, &xkb_error, &xkb_major, &xkb_minor))
    {
        glwtErrorPrintf("Xkb extension missing");
        goto error;
    }

    if((glwt.x11.xim = XOpenIM(glwt.x11.display, NULL, NULL, NULL)) == NULL)
    {
        glwtErrorPrintf("XOpenIM failed");
        goto error;
    }

    if(init_x11_atoms() != 0)
        goto error;

#ifdef GLWT_USE_EGL
    glwt.egl.display = eglGetDisplay(glwt.x11.display);
    if(glwtInitEGL(config) != 0)
#else
    if(glwtInitGLX(config) != 0)
#endif
        goto error;

    XVisualInfo template;
#ifdef GLWT_USE_EGL
    template.visualid = glwt.egl.visual_id;
#else
    template.visualid = glwt.glx.visual_id;
#endif

    int num_vis;
    XVisualInfo *vi = XGetVisualInfo(glwt.x11.display, VisualIDMask, &template, &num_vis);
    if(!vi || num_vis < 1)
    {
        XFree(vi);
        glwtErrorPrintf("XGetVisualInfo failed");
        goto error;
    }

    glwt.x11.visual = vi->visual;
    glwt.x11.depth = vi->depth;
    XFree(vi);

    if((glwt.x11.colormap = XCreateColormap(
        glwt.x11.display,
        RootWindow(glwt.x11.display, glwt.x11.screen_num),
        glwt.x11.visual,
        AllocNone)) == 0)
    {
        glwtErrorPrintf("XCreateColormap failed");
        goto error;
    }

    return 0;
error:
    glwtQuit();
    return -1;
}

void glwtQuit()
{
#ifdef GLWT_USE_EGL
    glwtQuitEGL();
#else
    glwtQuitGLX();
#endif

    if(glwt.x11.display)
    {
        if(glwt.x11.colormap)
            XFreeColormap(glwt.x11.display, glwt.x11.colormap);
        if(glwt.x11.xim)
            XCloseIM(glwt.x11.xim);
        XCloseDisplay(glwt.x11.display);
    }

    memset(&glwt, 0, sizeof(struct glwt));
}
