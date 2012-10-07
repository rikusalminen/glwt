cmake_minimum_required(VERSION 2.8)
project(glwt C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type for make-style systems" FORCE)
endif()

option(GLWT_BUILD_TESTS "Build GLWT tests" OFF)
mark_as_advanced(GLWT_BUILD_TESTS)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

if(BUILD_SHARED_LIBS)
    message(FATAL_ERROR "TODO: building shared libs not supported!")
endif()

if (CMAKE_COMPILER_IS_GNUCC)
    add_definitions(-std=gnu99)
    add_definitions(-W -Wall -Wextra)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(ext)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/ext/glxw/include)
link_directories(${CMAKE_CURRENT_BINARY_DIR}/ext/glxw)

set(GLWT_DEPENDENCY_LIBRARIES)
mark_as_advanced(GLWT_DEPENDENCY_LIBRARIES)
add_subdirectory(src)

set(GLWT_LIBRARIES ${GLWT_DEPENDENCY_LIBRARIES} glwt)
mark_as_advanced(GLWT_LIBRARIES)

if(GLWT_BUILD_TESTS)
    add_subdirectory(test)
endif()
