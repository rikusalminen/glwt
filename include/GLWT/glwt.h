#ifndef GLWT_glwt_h
#define GLWT_glwt_h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct GLWTConfig
{
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
