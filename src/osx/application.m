#import <GLWT/glwt.h>
#import <GLWT/glwt_keysym.h>
#import <glwt_internal.h>

#import <Cocoa/Cocoa.h>

#import "application.h"
#import "window.h"

//=================================================================
// Application class functions
//=================================================================

@implementation GLWTApplication
@synthesize modifier_flags;

// From http://cocoadev.com/index.pl?GameKeyboardHandlingAlmost
// This works around an AppKit bug, where key up events while holding
// down the command key don't get sent to the key window.
- (void)sendEvent:(NSEvent *)event
{
    if ([event type] == NSKeyUp && ([event modifierFlags] & NSCommandKeyMask))
        [[self keyWindow] sendEvent:event];
    else
        [super sendEvent:event];
}

@end

//=================================================================
// GLWT public functions
//=================================================================
int glwtEventHandle(int wait)
{
    int events = 0;
    NSUInteger mask = UINT_MAX; //NSAnyEventMask;

    NSDate* limitDate = nil;
    if(wait) limitDate = [NSDate distantFuture];
    else limitDate = [NSDate distantPast];


    do
    {
        NSEvent* event = [NSApp nextEventMatchingMask: mask
                                            untilDate: limitDate
                                               inMode: NSDefaultRunLoopMode
                                              dequeue: YES];

        [glwt.osx.app sendEvent:event];

        ++events;
    } while(events == 0 && wait);

    [glwt.osx.autorelease_pool drain];
    glwt.osx.autorelease_pool = [NSAutoreleasePool new];

    return 0;
}
