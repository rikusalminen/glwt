#ifndef GLWT_osx_window_h
#define GLWT_osx_window_h

//=================================================================
// OpenGLView class
//=================================================================

@interface GLWTView : NSView {
    GLWTWindow      *glwt_window;
    NSOpenGLContext *gl_context;
    NSTrackingArea  *tracking_area;
}
@property (assign) NSOpenGLContext *gl_context;
- (id)initWithFrame:(NSRect)frame andGLWTWindow:(GLWTWindow *)win;
@end


//=================================================================
// Window class
//=================================================================

@interface GLWTNSWindow : NSWindow {
    GLWTView *gl_view;
    GLWTWindow *glwt_window;
}
@property (assign) GLWTView *gl_view;
@property (assign) GLWTWindow *glwt_window;

@end


//=================================================================
// Window delegate class
//=================================================================

@interface GLWTNSWindowDelegate : NSObject {
    GLWTWindow *glwt_window;
}

@end

#endif // GLWT_osx_window_h
