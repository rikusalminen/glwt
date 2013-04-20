Building GLWT on Android NDK
============================

First you must generate GLXW headers and source code.

    $ cd ext/glxw
    $ python glxw_gen.py --with-platform " " --api egl
    $ python glxw_gen.py --with-platform " " --api gles2
    $ cd ../..

Add the superdirectory of glwt to NDK_MODULE_PATH.
Use import-module in your Android.mk to add GLWT to the build.
Build your using the Android NDK as usual.

To build the test app, run the following in glwt/test/android :

    $ NDK_MODULE_PATH=$PWD/../../.. ndk-build
