#include <GLWT/glwt.h>
#include <glwt_internal.h>

GLWTWindow *glwtWindowCreate(
    const char *title,
    int width, int height,
    GLWTWindow *share,
    void (*win_callback)(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata),
    void *userdata)
{
    (void)title;

    GLWTWindow *win = calloc(1, sizeof(GLWTWindow));
    if(!win)
        return 0;

    win->win_callback = win_callback;
    win->userdata = userdata;

    DISPMANX_UPDATE_HANDLE_T dispman_update;
    VC_RECT_T dst_rect;
    VC_RECT_T src_rect;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = glwt.rpi.width;
    dst_rect.height = glwt.rpi.height;

    width = glwt.rpi.width;
    height = glwt.rpi.height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = width << 16;
    src_rect.height = height << 16;

    dispman_update = vc_dispmanx_update_start( 0 );

    VC_DISPMANX_ALPHA_T alpha_flags;
    alpha_flags.flags = DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS;
    alpha_flags.opacity = 255;
    alpha_flags.mask = 0;

    win->rpi.nativewindow.element = vc_dispmanx_element_add(
        dispman_update,
        glwt.rpi.dispman_display,
        0 /*layer*/,
        &dst_rect,
        0 /*src*/,
        &src_rect,
        DISPMANX_PROTECTION_NONE,
        &alpha_flags /*alpha*/,
        0 /*clamp*/,
        (DISPMANX_TRANSFORM_T)0 /*transform*/);

    win->rpi.nativewindow.width = width;
    win->rpi.nativewindow.height = height;
    vc_dispmanx_update_submit_sync( dispman_update );

    if(glwtWindowCreateEGL(win, share, &win->rpi.nativewindow) != 0)
        goto error;

    return win;
error:
    glwtWindowDestroy(win);
    return 0;
}

void glwtWindowDestroy(GLWTWindow *win)
{
    if(!win)
        return;

    glwtWindowDestroyEGL(win);

    DISPMANX_UPDATE_HANDLE_T dispman_update = vc_dispmanx_update_start( 0 );
    vc_dispmanx_element_remove(dispman_update, win->rpi.nativewindow.element);
    vc_dispmanx_update_submit_sync( dispman_update );

    free(win);
}

void glwtWindowShow(GLWTWindow *win, int show)
{
    (void)win; (void)show;
}

void glwtWindowSetTitle(GLWTWindow *win, const char *title)
{
    (void)win; (void)title;
}
