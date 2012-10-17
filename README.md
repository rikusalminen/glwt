GLWT OpenGL windowing
=====================

GLWT is a lightweight cross platform windowing library for OpenGL that deals
with the platform specific parts of window and OpenGL context creation.

Building GLWT
-------------

0. Compile time requirements: CMake 2.8 and Python 2.7 (for downloading and
   generating GL extension loader).

1. Get GLWT and GLXW source code using git

        $ git clone git://github.com/rikusalminen/glwt.git
        $ git submodule init
        $ git submodule update

2. Use CMake to configure build environment

        $ mkdir build ; cd build
        $ cmake path/to/glwt/source -DGLWT_BUILD_TESTS=ON

3. Build GLWT using Make

        $ make

4. Run tests

        $ ./test/glwt_events

License
-------

GLWT is licensed under the zlib license. See file LICENSE for details.
