#import <GLWT/glwt.h>
#import <glwt_internal.h>

#import "application.h"

int createPixelFormat(const GLWTConfig *config)
{
    if(config &&
       (config->api & GLWT_API_MASK) != GLWT_API_ANY &&
       (config->api & GLWT_API_MASK) != GLWT_API_OPENGL)
    {
        glwtErrorPrintf("NSOpenGL can only initialize OpenGL profiles");
        return -1;
    }

    if(config &&
       config->api_version_major == 3 &&
       (config->api & GLWT_PROFILE_MASK) != GLWT_PROFILE_CORE)
    {
        glwtErrorPrintf("OSX only supports Core profile for OpenGL 3.2 contexts");
        return -1;
    }

    if (config &&
        ((config->api_version_major == 3 && config->api_version_minor != 2) ||
        config->api_version_major > 3))
    {
        glwtErrorPrintf("OSX only supports OpenGL versions up to 2.1 "
                        "and 3.2 Core profile");
        return -1;
    }

    glwt.api = config ? config->api : 0;
    glwt.api_version_major = config ? config->api_version_major : 0;
    glwt.api_version_minor = config ? config->api_version_minor : 0;

    int colorBits = 0;
    int core = 1;
    if(config)
    {
        colorBits = config->red_bits + config->green_bits + config->blue_bits;
        core = (config->api_version_major == 3 &&
                (config->api & GLWT_PROFILE_MASK) == GLWT_PROFILE_CORE) ||
                (config->api_version_major == 0);
    }

    int attribute = 0;
    NSOpenGLPixelFormatAttribute attribs[] = {
        NSOpenGLPFADoubleBuffer,
        NSOpenGLPFAColorSize, colorBits,
        NSOpenGLPFADepthSize, config ? config->depth_bits : 0,
        NSOpenGLPFAStencilSize, config ? config->stencil_bits : 0,
        NSOpenGLPFASampleBuffers, config ? config->sample_buffers : 0,
        NSOpenGLPFASamples, config ? config->samples : 0,
#if MAC_OSX_VERSION_MAX_ALLOWED >= 1070
        core ? NSOpenGLPFAOpenGLProfile : 0,
        core ? NSOpenGLPFAProfileVersion3_2Core : 0,
#endif
        0
    };

    glwt.osx.pixel_format = [[NSOpenGLPixelFormat alloc] initWithAttributes:attribs];
    if(glwt.osx.pixel_format == nil)
    {
        glwtErrorPrintf("Failed to create NSOpenGLPixelFormat");
        return -1;
    }

    return 0;
}

int glwtInit(const GLWTConfig *config,
             void (*error_callback)(const char *msg, void *userdata),
             void *userdata)
{
    glwt.error_callback = error_callback;
    glwt.userdata = userdata;

    if(createPixelFormat(config) < 0)
        return -1;

    glwt.osx.app = [GLWTApplication sharedApplication];
    ((GLWTApplication *)glwt.osx.app).modifier_flags = [NSEvent modifierFlags] & NSDeviceIndependentModifierFlagsMask;
    glwt.osx.autorelease_pool = [[NSAutoreleasePool alloc] init];

    /*
     If ran outside of application bundle, system assumes a
     command-line application or similiar. Tell system that we
     are actually a gui application that likes to have a dock icon etc.
     */
    ProcessSerialNumber psn;
    GetCurrentProcess(&psn);
    TransformProcessType(&psn, kProcessTransformToForegroundApplication);
    SetFrontProcess(&psn);

    [NSEvent setMouseCoalescingEnabled:NO];
    [glwt.osx.app finishLaunching];

    return 0;
}

void glwtQuit()
{
    if (glwt.osx.app)
    {
        [glwt.osx.app terminate:nil];
        [glwt.osx.app release];
        glwt.osx.app = nil;
    }
    if(glwt.osx.autorelease_pool)
    {
        [glwt.osx.autorelease_pool drain];
        glwt.osx.autorelease_pool = nil;
    }
}
