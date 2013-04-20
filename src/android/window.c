#include <stdlib.h>

#include <glwt_internal.h>

GLWTWindow *glwtWindowCreate(
    const char *title,
    int width, int height,
    GLWTWindow *share,
    void (*win_callback)(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata),
    void *userdata
    )
{
    (void)title;
    (void)width; (void)height;

    GLWTWindow *win = calloc(1, sizeof(GLWTWindow));
    if(!win)
        return 0;

    win->win_callback = win_callback;
    win->userdata = userdata;

    if(glwtWindowCreateContextEGL(win, share) != 0)
        goto error;

    return win;
error:
    glwtWindowDestroy(win);
    return 0;
}

void glwtWindowDestroy(GLWTWindow *win)
{
    glwtWindowDestroyEGL(win);

    free(win);
}

void glwtWindowShow(GLWTWindow *window, int show)
{
    (void)window;
    (void)show;
}
