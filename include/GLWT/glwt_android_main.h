#ifdef ANDROID

#include <android_native_app_glue.h>

int main(int argc, char *argv[]);

#ifdef __CPLUSPLUS
extern "C" {
#endif

void glwtAndroidApp(struct android_app *android_app);

void android_main(struct android_app *android_app)
{
    // Make sure glue isn't stripped.
    app_dummy();
    glwtAndroidApp(android_app);

    char *argv[] = { "glwt", 0 };
    main(1, argv);
}

#ifdef __CPLUSPLUS
}
#endif

#endif
