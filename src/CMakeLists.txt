include_directories(${CMAKE_CURRENT_SOURCE_DIR})
set(GLWT_SOURCES common/glwt.c)

option(GLWT_USE_EGL "Use EGL" OFF)
mark_as_advanced(GLWT_USE_EGL)

if(GLWT_USE_EGL)
    find_library(EGL_LIBRARY EGL)
    mark_as_advanced(EGL_LIBRARY)
    list(APPEND GLWT_DEPENDENCY_LIBRARIES ${EGL_LIBRARY})
    add_definitions(-DGLWT_USE_EGL)
else()
    find_package(OpenGL REQUIRED)
    include_directories(${OPENGL_INCLUDE_DIR})
    list(APPEND GLWT_DEPENDENCY_LIBRARIES ${OPENGL_gl_LIBRARY})
endif()

if(WIN32)
    list(APPEND GLWT_SOURCES win32/glwt_win32.c)

    if(NOT GLWT_USE_EGL)
        list(APPEND GLWT_SOURCES wgl/glwt_wgl.h)
    endif()

elseif (APPLE)
    list(APPEND GLWT_SOURCES osx/glwt_osx.c)
else()
    find_package(X11 REQUIRED)
    list(APPEND GLWT_DEPENDENCY_LIBRARIES ${X11_LIBRARIES})
    include_directories(${X11_X11_INCLUDE_PATH})

    find_library(DL_LIBRARY dl)
    mark_as_advanced(DL_LIBRARY)
    if(DL_LIBRARY)
        list(APPEND GLWT_DEPENDENCY_LIBRARIES ${DL_LIBRARY})
    endif()

    list(APPEND GLWT_SOURCES x11/glwt_x11.c)

    if(NOT GLWT_USE_EGL)
        list(APPEND GLWT_SOURCES glx/glwt_glx.c)
    endif()
endif()

add_library(glwt ${GLWT_SOURCES})
target_link_libraries(glwt ${GLWT_DEPENDENCY_LIBRARIES})
