#ifndef GLWT_glwt_h
#define GLWT_glwt_h

#include <GLWT/glwt_keysym.h>

#ifdef __cplusplus
extern "C" {
#endif

#define GLWT_API_ANY                    0x00
#define GLWT_API_OPENGL                 0x01
#define GLWT_API_OPENGL_ES              0x02
#define GLWT_API_MASK                   0x0f

#define GLWT_PROFILE_CORE               0x00
#define GLWT_PROFILE_COMPATIBILITY      0x10
#define GLWT_PROFILE_DEBUG              0x20
#define GLWT_PROFILE_ROBUSTNESS         0x40
#define GLWT_PROFILE_MASK               0xf0

typedef struct GLWTConfig
{
    int red_bits, green_bits, blue_bits, alpha_bits;
    int depth_bits, stencil_bits;
    int samples, sample_buffers;
    int api;
    int api_version_major, api_version_minor;
} GLWTConfig;

typedef struct GLWTAppCallbacks
{
    void (*error_callback)(const char *msg, void *userdata);
    void* userdata;
} GLWTAppCallbacks;

typedef struct GLWTWindow GLWTWindow;

typedef struct GLWTWindowCallbacks
{
    void (*close_callback)(GLWTWindow *window, void *userdata);
    void (*expose_callback)(GLWTWindow *window, void *userdata);
    void (*resize_callback)(GLWTWindow *window, int width, int height, void *userdata);
    void (*show_callback)(GLWTWindow *window, int show, void *userdata);
    void (*focus_callback)(GLWTWindow *window, int focus, void *userdata);
    void (*key_callback)(GLWTWindow *window, int down, int keysym, int scancode, int mod, void *userdata);
    void (*motion_callback)(GLWTWindow *window, int x, int y, int buttons, void *userdata);
    void (*button_callback)(GLWTWindow *window, int down, int x, int y, int button, int mod, void *userdata);
    void (*mouseover_callback)(GLWTWindow *window, int enter, void *userdata);
    void* userdata;
} GLWTWindowCallbacks;

int glwtInit(const GLWTConfig *config, const GLWTAppCallbacks *app_callbacks);
void glwtQuit();

GLWTWindow *glwtWindowCreate(
    const char *title,
    int width, int height,
    const GLWTWindowCallbacks *win_callbacks,
    GLWTWindow *share);
void glwtWindowDestroy(GLWTWindow *window);

int glwtWindowClosed(GLWTWindow *window);
void glwtWindowShow(GLWTWindow *window, int show);

void glwtMakeCurrent(GLWTWindow *win);
void glwtSwapBuffers(GLWTWindow *win);
void glwtSwapInterval(GLWTWindow *win, int interval);
void glwtWindowGetSize(GLWTWindow *win, int *width, int *height);

int glwtEventHandle(int wait);

#ifdef __cplusplus
}
#endif

#endif
