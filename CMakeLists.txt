cmake_minimum_required(VERSION 3.1)
project(glfwPI CXX)
set(CMAKE_BUILD_TYPE Debug)

if(UNIX)
    add_definitions(-DTARGET_LINUX)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


set(CMAKE_POSITION_INDEPENDENT_CODE 1)


# Make sure we use C++14
# (mainly because of make_unique which they kinda forgot to add to c++11 )
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Include OpenGL
include(FindOpenGL)
include_directories(${OPENGL_INCLUDE_DIR})
list(APPEND DEPLIBS ${OPENGL_gl_LIBRARY} ${OPENGL_LIBRARIES})


# Include GLFW
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/depends/GLFW/add_GLFW.cmake")
include_directories(${GLFW_SRC_DIR}/include)
list(APPEND DEPLIBS glfw ${GLFW_LIBRARIES})


# Include PictureIt
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/depends/PictureIt/add_PictureIt.cmake")
list(APPEND DEPLIBS PictureIt::PictureIt)

# Use local PictureIt source-tree instead
#add_subdirectory("3rdParty/PictureIt")
#include_directories("3rdParty/PictureIt/include")
#list(APPEND DEPLIBS PictureIt::PictureIt)


# Include PulseAudio
find_package(PulseAudio REQUIRED)
include_directories(${PULSEAUDIO_INCLUDE_DIR})
list(APPEND DEPLIBS ${PULSEAUDIO_LIBRARY})


set(SOURCES src/main.cpp
            src/pulse.cpp)

set(HEADERS src/pulse.hpp)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} ${DEPLIBS})


install(TARGETS ${PROJECT_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/bin")
