cmake_minimum_required(VERSION 3.0)
project(glfwPI CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


# We need C++11
find_package(CXX11 REQUIRED)


set(DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")


# Include PictureIt
add_subdirectory("${DEPENDENCIES_DIR}/PictureIt")
include_directories("${DEPENDENCIES_DIR}/PictureIt/include")
list(APPEND DEPLIBS PictureIt)


# Include GLFW 
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("${DEPENDENCIES_DIR}/glfw")
include_directories("${DEPENDENCIES_DIR}/glfw/include")
list(APPEND DEPLIBS glfw ${GLFW_LIBRARIES})


# Include libsndfile
find_package(libsndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIR})
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
list(APPEND DEPLIBS ${LIBSNDFILE_LIBRARY} ${LIBSNDFILE_LIBRARIES})


# Include PulseAudio
find_package(PulseAudio REQUIRED)
include_directories(${PULSEAUDIO_INCLUDE_DIR})
list(APPEND DEPLIBS ${PULSEAUDIO_LIBRARY})


# Include PulseAudioSimple
find_package(PulseAudioSimple REQUIRED)
include_directories(${PULSEAUDIOSIMPLE_INCLUDE_DIR})
list(APPEND DEPLIBS ${PULSEAUDIOSIMPLE_LIBRARY})


add_executable(${PROJECT_NAME} src/main.cpp src/audio.cpp)
target_link_libraries(${PROJECT_NAME} ${DEPLIBS})


install(TARGETS ${PROJECT_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/bin")
