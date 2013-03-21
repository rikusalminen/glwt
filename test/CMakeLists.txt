link_libraries(${GLWT_LIBRARIES})

add_executable(glwt_simplest WIN32 glwt_simplest.c)
add_executable(glwt_events glwt_events.c)

if (MSVC)
    set_target_properties(glwt_simplest PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif(MSVC)
