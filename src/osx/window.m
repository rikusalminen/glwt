#import <GLWT/glwt.h>
#import <glwt_internal.h>

#import <Cocoa/Cocoa.h>
#import <CoreVideo/CoreVideo.h>

#import "window.h"

//=================================================================
// Helper functions
//=================================================================

static int convertKeysym(unsigned int keyCode)
{
    // Keyboard symbol translation table
    static const unsigned int table[128] =
    {
        /* 00 */ GLWT_KEY_A,
        /* 01 */ GLWT_KEY_S,
        /* 02 */ GLWT_KEY_D,
        /* 03 */ GLWT_KEY_F,
        /* 04 */ GLWT_KEY_H,
        /* 05 */ GLWT_KEY_G,
        /* 06 */ GLWT_KEY_Z,
        /* 07 */ GLWT_KEY_X,
        /* 08 */ GLWT_KEY_C,
        /* 09 */ GLWT_KEY_V,
        /* 0a */ GLWT_KEY_UNKNOWN, // backtick
        /* 0b */ GLWT_KEY_B,
        /* 0c */ GLWT_KEY_Q,
        /* 0d */ GLWT_KEY_W,
        /* 0e */ GLWT_KEY_E,
        /* 0f */ GLWT_KEY_R,
        /* 10 */ GLWT_KEY_Y,
        /* 11 */ GLWT_KEY_T,
        /* 12 */ GLWT_KEY_1,
        /* 13 */ GLWT_KEY_2,
        /* 14 */ GLWT_KEY_3,
        /* 15 */ GLWT_KEY_4,
        /* 16 */ GLWT_KEY_6,
        /* 17 */ GLWT_KEY_5,
        /* 18 */ GLWT_KEY_UNKNOWN, // equal sign
        /* 19 */ GLWT_KEY_9,
        /* 1a */ GLWT_KEY_7,
        /* 1b */ GLWT_KEY_MINUS,
        /* 1c */ GLWT_KEY_8,
        /* 1d */ GLWT_KEY_0,
        /* 1e */ GLWT_KEY_UNKNOWN, // right bracket
        /* 1f */ GLWT_KEY_O,
        /* 20 */ GLWT_KEY_U,
        /* 21 */ GLWT_KEY_UNKNOWN, // left bracket
        /* 22 */ GLWT_KEY_I,
        /* 23 */ GLWT_KEY_P,
        /* 24 */ GLWT_KEY_RETURN,
        /* 25 */ GLWT_KEY_L,
        /* 26 */ GLWT_KEY_J,
        /* 27 */ GLWT_KEY_UNKNOWN, // apostrophe
        /* 28 */ GLWT_KEY_K,
        /* 29 */ GLWT_KEY_UNKNOWN, // semicolon
        /* 2a */ GLWT_KEY_UNKNOWN, // backslash
        /* 2b */ GLWT_KEY_COMMA,
        /* 2c */ GLWT_KEY_SLASH,
        /* 2d */ GLWT_KEY_N,
        /* 2e */ GLWT_KEY_M,
        /* 2f */ GLWT_KEY_PERIOD,
        /* 30 */ GLWT_KEY_TAB,
        /* 31 */ GLWT_KEY_SPACE,
        /* 32 */ GLWT_KEY_UNKNOWN,
        /* 33 */ GLWT_KEY_BACKSPACE,
        /* 34 */ GLWT_KEY_UNKNOWN,
        /* 35 */ GLWT_KEY_ESCAPE,
        /* 36 */ GLWT_KEY_RSUPER,
        /* 37 */ GLWT_KEY_LSUPER,
        /* 38 */ GLWT_KEY_LSHIFT,
        /* 39 */ GLWT_KEY_CAPS_LOCK,
        /* 3a */ GLWT_KEY_LALT,
        /* 3b */ GLWT_KEY_LCTRL,
        /* 3c */ GLWT_KEY_RSHIFT,
        /* 3d */ GLWT_KEY_RALT,
        /* 3e */ GLWT_KEY_RCTRL,
        /* 3f */ GLWT_KEY_UNKNOWN, // Fn
        /* 40 */ GLWT_KEY_UNKNOWN, // F17
        /* 41 */ GLWT_KEY_KEYPAD_SEPARATOR, // keypad comma/delete
        /* 42 */ GLWT_KEY_UNKNOWN,
        /* 43 */ GLWT_KEY_KEYPAD_MULTIPLY,
        /* 44 */ GLWT_KEY_UNKNOWN,
        /* 45 */ GLWT_KEY_KEYPAD_PLUS,
        /* 46 */ GLWT_KEY_UNKNOWN,
        /* 47 */ GLWT_KEY_NUM_LOCK, // Really KeypadClear
        /* 48 */ GLWT_KEY_UNKNOWN, // VolumeUp
        /* 49 */ GLWT_KEY_UNKNOWN, // VolumeDown
        /* 4a */ GLWT_KEY_UNKNOWN, // Mute
        /* 4b */ GLWT_KEY_KEYPAD_DIVIDE,
        /* 4c */ GLWT_KEY_KEYPAD_ENTER,
        /* 4d */ GLWT_KEY_UNKNOWN,
        /* 4e */ GLWT_KEY_KEYPAD_MINUS,
        /* 4f */ GLWT_KEY_UNKNOWN, // F18
        /* 50 */ GLWT_KEY_UNKNOWN, // F19
        /* 51 */ GLWT_KEY_UNKNOWN, // KEYPAD equal
        /* 52 */ GLWT_KEY_KEYPAD_0,
        /* 53 */ GLWT_KEY_KEYPAD_1,
        /* 54 */ GLWT_KEY_KEYPAD_2,
        /* 55 */ GLWT_KEY_KEYPAD_3,
        /* 56 */ GLWT_KEY_KEYPAD_4,
        /* 57 */ GLWT_KEY_KEYPAD_5,
        /* 58 */ GLWT_KEY_KEYPAD_6,
        /* 59 */ GLWT_KEY_KEYPAD_7,
        /* 5a */ GLWT_KEY_UNKNOWN, // F20
        /* 5b */ GLWT_KEY_KEYPAD_8,
        /* 5c */ GLWT_KEY_KEYPAD_9,
        /* 5d */ GLWT_KEY_UNKNOWN,
        /* 5e */ GLWT_KEY_UNKNOWN,
        /* 5f */ GLWT_KEY_UNKNOWN,
        /* 60 */ GLWT_KEY_F5,
        /* 61 */ GLWT_KEY_F6,
        /* 62 */ GLWT_KEY_F7,
        /* 63 */ GLWT_KEY_F3,
        /* 64 */ GLWT_KEY_F8,
        /* 65 */ GLWT_KEY_F9,
        /* 66 */ GLWT_KEY_UNKNOWN,
        /* 67 */ GLWT_KEY_F11,
        /* 68 */ GLWT_KEY_UNKNOWN,
        /* 69 */ GLWT_KEY_UNKNOWN, // F13
        /* 6a */ GLWT_KEY_UNKNOWN, // F16
        /* 6b */ GLWT_KEY_UNKNOWN, // F14
        /* 6c */ GLWT_KEY_UNKNOWN,
        /* 6d */ GLWT_KEY_F10,
        /* 6e */ GLWT_KEY_UNKNOWN,
        /* 6f */ GLWT_KEY_F12,
        /* 70 */ GLWT_KEY_UNKNOWN,
        /* 71 */ GLWT_KEY_UNKNOWN, // F15
        /* 72 */ GLWT_KEY_INSERT, // Really Help
        /* 73 */ GLWT_KEY_HOME,
        /* 74 */ GLWT_KEY_PAGE_UP,
        /* 75 */ GLWT_KEY_DELETE,
        /* 76 */ GLWT_KEY_F4,
        /* 77 */ GLWT_KEY_END,
        /* 78 */ GLWT_KEY_F2,
        /* 79 */ GLWT_KEY_PAGE_DOWN,
        /* 7a */ GLWT_KEY_F1,
        /* 7b */ GLWT_KEY_LEFT,
        /* 7c */ GLWT_KEY_RIGHT,
        /* 7d */ GLWT_KEY_DOWN,
        /* 7e */ GLWT_KEY_UP,
        /* 7f */ GLWT_KEY_UNKNOWN,
    };

    if (keyCode >= 128)
        return GLWT_KEY_UNKNOWN;

    return table[keyCode];
}

static unsigned int convertModifiers(unsigned int mods)
{
    unsigned int mask = 0;

    if(mods & NSAlphaShiftKeyMask)
        mask |= GLWT_MOD_CAPS_LOCK;

    if(mods & NSShiftKeyMask)
        mask |= GLWT_MOD_SHIFT;

    if(mods & NSControlKeyMask)
        mask |= GLWT_MOD_CTRL;

    if(mods & NSAlternateKeyMask)
        mask |= GLWT_MOD_ALT;

    if(mods & NSCommandKeyMask)
        mask |= GLWT_MOD_SUPER;

    return mask;
}

//=================================================================
// OpenGLView class functions
//=================================================================

@implementation GLWTView
@synthesize gl_context;

- (id)initWithFrame:(NSRect)frame andGLWTWindow:(GLWTWindow *)win
{
    if((self = [super initWithFrame:frame]))
    {
        gl_context = nil;
        glwt_window = win;
        tracking_area = nil;

        [self updateTrackingAreas];
    }

    return self;
}

- (void)dealloc
{
    [tracking_area release];
    [super dealloc];
}

- (void)updateTrackingAreas
{
    if(tracking_area != nil)
    {
        [self removeTrackingArea:tracking_area];
        [tracking_area release];
    }

    NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited | NSTrackingActiveAlways | NSTrackingInVisibleRect;

    tracking_area = [[NSTrackingArea alloc] initWithRect:[self bounds] options:options owner:self userInfo:nil];
    [self addTrackingArea:tracking_area];
    [super updateTrackingAreas];
}

- (BOOL)isOpaque
{
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (void)keyDown:(NSEvent *)event
{
    if(!glwt_window->win_callback)
        return;

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_KEY_DOWN;
    e.key.keysym = convertKeysym(event.keyCode);
    e.key.scancode = event.keyCode;
    e.key.mod = convertModifiers([event modifierFlags]);

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);

    for(unsigned int idx = 0; idx < [[event characters] length]; ++idx)
    {
        GLWTWindowEvent e;

        e.type = GLWT_WINDOW_CHARACTER_INPUT;
        e.window = glwt_window;
        e.character.unicode = [[event characters] characterAtIndex:idx];
        glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
    }
}

- (void)keyUp:(NSEvent *)event
{
    if(!glwt_window->win_callback)
        return;

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_KEY_UP;
    e.key.keysym = convertKeysym(event.keyCode);
    e.key.scancode = event.keyCode;
    e.key.mod = convertModifiers([event modifierFlags]);

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)flagsChanged:(NSEvent *)event
{
    NSUInteger newModifiers = [event modifierFlags] & NSDeviceIndependentModifierFlagsMask;
    int down = (newModifiers > glwt_window->osx.modifier_flags);
    glwt_window->osx.modifier_flags = newModifiers;

    if(!glwt_window->win_callback)
        return;

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = (down) ? GLWT_WINDOW_KEY_DOWN : GLWT_WINDOW_KEY_UP;
    e.key.keysym = convertKeysym(event.keyCode);
    e.key.scancode = event.keyCode;
    e.key.mod = convertModifiers(newModifiers);

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)mouseMoved:(NSEvent *)event
{
    if (!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_MOUSE_MOTION;
    e.motion.x = pos.x;
    e.motion.y = frame.size.height - pos.y;
    e.motion.buttons = 0;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)mouseDragged:(NSEvent *)event
{
    if (!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_MOUSE_MOTION;
    e.motion.x = pos.x;
    e.motion.y = frame.size.height - pos.y;
    e.motion.buttons = 1;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)rightMouseDragged:(NSEvent *)event
{
    if (!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_MOUSE_MOTION;
    e.motion.x = pos.x;
    e.motion.y = frame.size.height - pos.y;
    e.motion.buttons = 2;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)otherMouseDragged:(NSEvent *)event
{
    if (!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_MOUSE_MOTION;
    e.motion.x = pos.x;
    e.motion.y = frame.size.height - pos.y;
    e.motion.buttons = [event buttonNumber];

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)mouseDown:(NSEvent *)event
{
    if(!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_BUTTON_DOWN;
    e.button.x = pos.x;
    e.button.y = frame.size.height - pos.y;
    e.button.button = 1;
    e.button.mod = convertModifiers([event modifierFlags]);

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)mouseUp:(NSEvent *)event
{
    if(!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_BUTTON_UP;
    e.button.x = pos.x;
    e.button.y = frame.size.height - pos.y;
    e.button.button = 1;
    e.button.mod = convertModifiers([event modifierFlags]);

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)rightMouseDown:(NSEvent *)event
{
    if(!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_BUTTON_DOWN;
    e.button.x = pos.x;
    e.button.y = frame.size.height - pos.y;
    e.button.button = 2;
    e.button.mod = convertModifiers([event modifierFlags]);

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)rightMouseUp:(NSEvent *)event
{
    if(!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_BUTTON_UP;
    e.button.x = pos.x;
    e.button.y = frame.size.height - pos.y;
    e.button.button = 2;
    e.button.mod = convertModifiers([event modifierFlags]);

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)otherMouseDown:(NSEvent *)event
{
    if(!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_BUTTON_DOWN;
    e.button.x = pos.x;
    e.button.y = frame.size.height - pos.y;
    e.button.button = [event buttonNumber];
    e.button.mod = convertModifiers([event modifierFlags]);

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)otherMouseUp:(NSEvent *)event
{
    if(!glwt_window->win_callback)
        return;

    NSPoint pos = [self convertPoint:[event locationInWindow] fromView:nil];
    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_BUTTON_UP;
    e.button.x = pos.x;
    e.button.y = frame.size.height - pos.y;
    e.button.button = [event buttonNumber];
    e.button.mod = convertModifiers([event modifierFlags]);

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)mouseEntered:(NSEvent *)event
{
    (void)event;

    if(!glwt_window->win_callback)
        return;

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_MOUSE_ENTER;
    e.dummy.dummy = 0;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)mouseExited:(NSEvent *)event
{
    (void)event;

    if(!glwt_window->win_callback)
        return;

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_MOUSE_LEAVE;
    e.dummy.dummy = 0;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)drawRect:(NSRect)dirtyRect
{
    (void)dirtyRect;

    if(!glwt_window->win_callback)
        return;

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_EXPOSE;
    e.dummy.dummy = 0;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

@end

//=================================================================
// Window class functions
//=================================================================

@implementation GLWTNSWindow
@synthesize gl_view;
@synthesize glwt_window;

- (BOOL)canBecomeKeyWindow
{
    return YES;
}

- (BOOL)canBecomeMainWindow
{
    return YES;
}

@end

//=================================================================
// Window delegate class functions
//=================================================================
@implementation GLWTNSWindowDelegate
- (id)initWithGLWTWindow:(GLWTWindow *)win
{
    if((self = [super init]))
    {
        glwt_window = win;
    }

    return self;
}

- (BOOL)windowShouldClose:(id)sender
{
    (void)sender;
    glwt_window->closed = 1;

    if(!glwt_window->win_callback)
        return NO;

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_CLOSE;
    e.dummy.dummy = 0;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
    return NO;
}

- (void)windowDidBecomeKey:(NSNotification *)notification
{
    (void)notification;

    if(!glwt_window->win_callback)
        return;

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_FOCUS_IN;
    e.dummy.dummy = 0;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)windowDidResignKey:(NSNotification *)notification
{
    (void)notification;

    if(!glwt_window->win_callback)
        return;

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_FOCUS_OUT;
    e.dummy.dummy = 0;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}


- (void)windowDidResize:(NSNotification *)notification
{
    (void)notification;

    [glwt_window->osx.ctx update];

    if(!glwt_window->win_callback)
        return;

    NSRect frame = [[glwt_window->osx.nswindow contentView] frame];

    GLWTWindowEvent e;
    e.window = glwt_window;
    e.type = GLWT_WINDOW_RESIZE;
    e.resize.width = frame.size.width;
    e.resize.height = frame.size.height;

    glwt_window->win_callback(glwt_window, &e, glwt_window->userdata);
}

- (void)windowDidMove:(NSNotification *)notification
{
    (void)notification;

    [glwt_window->osx.ctx update];

    // TODO: handle event?
}

- (void)windowDidMiniaturize:(NSNotification *)notification
{
    (void)notification;
    // TODO: handle event?
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{
    (void)notification;
    // TODO: handle event?
}

@end

//=================================================================
// GLWT public functions
//=================================================================

GLWTWindow *glwtWindowCreate(
    const char *title,
    int width, int height,
    GLWTWindow *share,
    void (*win_callback)(GLWTWindow *window, const GLWTWindowEvent *event, void *userdata),
    void *userdata)
{
    GLWTView *view = 0;
    GLWTWindow *win = calloc(1, sizeof(GLWTWindow));
    if (!win)
        goto error;

    win->win_callback = win_callback;
    win->userdata = userdata;

    unsigned int styleMask = NSTitledWindowMask | NSClosableWindowMask |
    NSMiniaturizableWindowMask | NSResizableWindowMask;

    win->osx.nswindow = [[GLWTNSWindow alloc]
                           initWithContentRect:NSMakeRect(0, 0, width, height)
                           styleMask:styleMask
                           backing:NSBackingStoreBuffered
                           defer:NO];
    if(!win->osx.nswindow)
    {
        glwtErrorPrintf("NSWindow initWithContentRect failed");
        goto error;
    }

    ((GLWTNSWindow *)win->osx.nswindow).glwt_window = win;
    [win->osx.nswindow center];
    win->osx.delegate = [[GLWTNSWindowDelegate alloc] initWithGLWTWindow:win];
    [win->osx.nswindow setDelegate:[win->osx.delegate retain]];
    [win->osx.nswindow setAcceptsMouseMovedEvents:YES];
    [win->osx.nswindow setTitle:[NSString stringWithUTF8String:title]];
    [win->osx.nswindow setReleasedWhenClosed:NO];

    view = [[GLWTView alloc] initWithFrame:[win->osx.nswindow frame]andGLWTWindow:win];
    if(!view)
    {
        glwtErrorPrintf("NSView initWithFrame failed");
        goto error;
    }
    win->osx.view = [view retain];
    [win->osx.nswindow setContentView:view];

    win->osx.ctx = [[NSOpenGLContext alloc]
                            initWithFormat:glwt.osx.pixel_format
                            shareContext: share ? share->osx.ctx : nil];

    if(!win->osx.ctx)
    {
        glwtErrorPrintf("Failed to create NSOpenGL Context");
        goto error;
    }
    [win->osx.ctx setView:[win->osx.nswindow contentView]];

    if([win->osx.nswindow respondsToSelector:@selector(setRestorable:)])
        [win->osx.nswindow setRestorable:NO];

    win->osx.modifier_flags = [NSEvent modifierFlags] & NSDeviceIndependentModifierFlagsMask;

    return win;

error:
    glwtWindowDestroy(win);
    return 0;
}

void glwtWindowDestroy(GLWTWindow *win)
{
    if(!win)
        return;
    if(win->osx.ctx)
       [win->osx.ctx release];
    if(win->osx.nswindow)
    {
        [win->osx.nswindow setDelegate:nil];
        [win->osx.delegate release];
        [win->osx.nswindow setContentView:nil];
        [win->osx.view release];
        [win->osx.nswindow close];
    }
    free(win);
}

void glwtWindowSetTitle(GLWTWindow *win, const char *title)
{
    [win->osx.nswindow setTitle:[NSString stringWithUTF8String:title]];
}

int glwtWindowGetSize(GLWTWindow *win, int *width, int *height)
{
    NSRect frame = [[win->osx.nswindow contentView] frame];
    *width = (int)frame.size.width;
    *height = (int)frame.size.height;
    return 0;
}

void glwtWindowShow(GLWTWindow *win, int show)
{
    if(show)
        [win->osx.nswindow makeKeyAndOrderFront:nil];
    else
        [win->osx.nswindow orderOut:nil];
}

int glwtMakeCurrent(GLWTWindow *win)
{
    if (!win)
        [NSOpenGLContext clearCurrentContext];
    else
        [win->osx.ctx makeCurrentContext];
    return 0;
}

int glwtSwapBuffers(GLWTWindow *win)
{
    [win->osx.ctx flushBuffer];
    return 0;
}

int glwtSwapInterval(GLWTWindow *win, int interval)
{
    GLint sync = interval;

    [win->osx.ctx setValues:&sync forParameter:NSOpenGLCPSwapInterval];
    return 0;
}
