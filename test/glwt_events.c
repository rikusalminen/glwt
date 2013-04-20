#include <stdio.h>
#include <ctype.h>

#include <GLWT/glwt.h>

#ifdef GLWT_TESTS_GLES
#include <GLXW/glxw_es2.h>
#else
#include <GLXW/glxw.h>
#endif

#define MAX_WINDOWS 16

static int encode_utf8(unsigned int codepoint, unsigned char *out)
{
    if((codepoint & ~0x7F) == 0)
    {
        out[0] = (codepoint & 0x7F);
        return 1;
    } else if((codepoint & ~0x7FF) == 0)
    {
        out[0] = (0xc0 | (codepoint & 0x7C0) >> 6);
        out[1] = (0x80 | (codepoint & 0x3F));
        return 2;
    } else if((codepoint & ~0xFFFF) == 0)
    {
        out[0] = (0xe0 | (codepoint & 0xF000) >> 12);
        out[1] = (0x80 | (codepoint & 0xFC) >> 6);
        out[2] = (0x80 | (codepoint & 0x3F));
        return 3;
    } else if((codepoint & ~0x3FFFF) == 0)
    {
        out[0] = (0xf0 | (codepoint & 0x1c0000) >> 18);
        out[1] = (0x80 | (codepoint & 0x3f00) >> 12);
        out[2] = (0x80 | (codepoint & 0xFC) >> 6);
        out[3] = (0x80 | (codepoint & 0x3F));
        return 4;
    }

    return 0;
}

static GLWTWindow *windows[MAX_WINDOWS] = {0};
static int windowsOpen = 0;

static int newWindow();

static void error_callback(const char *msg, void *userdata)
{
    (void)userdata;
    fprintf(stderr, "%s\n", msg);
}

static void window_callback(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata)
{
    (void)userdata;
    double time = glwtGetTime();

    switch(event->type)
    {
        case GLWT_WINDOW_CLOSE:
            printf("[%lf] Window %p closed\n", time, window);
            break;
        case GLWT_WINDOW_EXPOSE:
            printf("[%lf] Window %p exposed\n", time, window);
            {
                glwtMakeCurrent(window);
                glwtSwapInterval(window, 1);

                glxwInit();

                glClearColor(0.2f, 0.4f, 0.7f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                glwtSwapBuffers(window);
                glwtMakeCurrent(NULL);
            }
            break;
        case GLWT_WINDOW_RESIZE:
            printf("[%lf] Window %p resized  width: %d  height: %d\n", time,
                    window, event->resize.width, event->resize.height);
            break;
        case GLWT_WINDOW_SHOW:
        case GLWT_WINDOW_HIDE:
            printf("[%lf] Window %p %s\n", time, window,
                    (event->type == GLWT_WINDOW_SHOW) ? "show" : "hide");
            break;
        case GLWT_WINDOW_FOCUS_IN:
        case GLWT_WINDOW_FOCUS_OUT:
            printf("[%lf] Window %p focus %s\n", time, window,
                    (event->type == GLWT_WINDOW_FOCUS_IN) ? "in" : "out");
            break;
        case GLWT_WINDOW_KEY_UP:
        case GLWT_WINDOW_KEY_DOWN:
            printf("[%lf] Window %p key %s  keysym: 0x%x  scancode: %d  mod: %X\n",
                    time, window,
                    (event->type == GLWT_WINDOW_KEY_DOWN) ? "down" : "up",
                    event->key.keysym, event->key.scancode, event->key.mod);

            if(event->type == GLWT_WINDOW_KEY_DOWN
                && event->key.keysym == GLWT_KEY_RETURN)
            {
                newWindow();
            }
            break;
        case GLWT_WINDOW_BUTTON_UP:
        case GLWT_WINDOW_BUTTON_DOWN:
            printf("[%lf] Window %p button %s  x: %d  y: %d  button: %d  mod: %X\n",
                    time, window,
                    (event->type == GLWT_WINDOW_BUTTON_DOWN) ? "down" : "up",
                    event->button.x, event->button.y, event->button.button,
                    event->button.mod);
            break;
        case GLWT_WINDOW_MOUSE_MOTION:
            printf("[%lf] Window %p motion  x: %d  y: %d  buttons: %X\n", time,
                    window, event->motion.x, event->motion.y, event->motion.buttons);
            break;
        case GLWT_WINDOW_MOUSE_ENTER:
        case GLWT_WINDOW_MOUSE_LEAVE:
            printf("[%lf] Window %p mouse %s\n", time, window,
                    (event->type == GLWT_WINDOW_MOUSE_ENTER) ? "enter" : "leave");
            break;
        case GLWT_WINDOW_CHARACTER_INPUT:
            printf("[%lf] Window %p character input 0x%x (%c)\n", time, window,
                    event->character.unicode,
                    (event->character.unicode < 128 && isprint(event->character.unicode)) ?
                    event->character.unicode : ' ');
            if(event->character.unicode >= 128 || isprint(event->character.unicode))
            {
                int bytes = 0;
                char buf[5];

                bytes = encode_utf8(event->character.unicode, (unsigned char*)buf);
                buf[bytes] = 0;

                glwtWindowSetTitle(window, buf);
            }
            break;
        default:
            break;
    }
}

static int newWindow()
{
    int width, height;
    int id = 0;
    for (int i = 0; i < MAX_WINDOWS; ++i)
    {
        if(!windows[i])
        {
            windows[i] = glwtWindowCreate("", 400, 300, NULL, window_callback, NULL);
            if(!windows[i])
            {
                printf("Could not open a window!\n");
                return 0;
            }
            id = i;
            break;
        }
    }

    ++windowsOpen;

    glwtWindowSetTitle(windows[id], "GLWT Events test");

    glwtWindowShow(windows[id], 1);
    glwtMakeCurrent(windows[id]);
    glwtSwapInterval(windows[id], 1);

    glxwInit();

    printf("%s\n", (const char*)glGetString(GL_VERSION));

    glwtWindowGetSize(windows[id], &width, &height);
    printf("Window %p size: %d x %d\n", windows[id], width, height);

    return 1;
}

int main(int argc, char *argv[])
{
    int err = -1;
    GLWTConfig glwt_config = {
        0, 0, 0, 0,
        0, 0,
        0, 0,
#ifdef GLWT_TESTS_GLES
        GLWT_API_OPENGL_ES | GLWT_PROFILE_DEBUG,
        2, 0,
#else
        GLWT_API_OPENGL | GLWT_PROFILE_DEBUG,
        0, 0,
#endif
    };

    (void)argc; (void)argv;

    if(glwtInit(&glwt_config, error_callback, NULL) != 0)
        goto error;

    if(!newWindow())
        goto error;

    while(windowsOpen)
    {
        if(glwtEventHandle(1) != 0)
            goto error;

        for (int i = 0; i < MAX_WINDOWS; ++i)
        {
            if(windows[i] && glwtWindowClosed(windows[i]))
            {
                glwtWindowDestroy(windows[i]);
                --windowsOpen;
                windows[i] = 0;
            }
        }
    }

    err = 0;
error:
    for (int i = 0; i < MAX_WINDOWS; ++i)
    {
        if(windows[i])
            glwtWindowDestroy(windows[i]);
    }
    glwtQuit();
    return err;
}
