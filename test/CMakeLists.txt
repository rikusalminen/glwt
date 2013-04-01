option(GLWT_TESTS_GLES OFF)
mark_as_advanced(GLWT_TESTS_GLES)

if(GLWT_TESTS_GLES)
    if(GLWT_USE_EGL)
        find_library(GLESv2_LIBRARY GLESv2 REQUIRED)
        link_libraries(${GLESv2_LIBRARY})
    endif()

    add_definitions(-DGLWT_TESTS_GLES)
    link_libraries(glxw_es2)
else()
    link_libraries(glxw)
endif()

link_libraries(${GLWT_LIBRARIES})

add_executable(glwt_simplest WIN32 glwt_simplest.c)
add_executable(glwt_events glwt_events.c)

if (MSVC)
    set_target_properties(glwt_simplest PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif(MSVC)
