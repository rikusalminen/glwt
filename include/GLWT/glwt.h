#ifndef GLWT_glwt_h
#define GLWT_glwt_h

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

int glwtInit(const GLWTConfig *config, const GLWTAppCallbacks *app_callbacks);
void glwtQuit();

#ifdef __cplusplus
}
#endif

#endif
