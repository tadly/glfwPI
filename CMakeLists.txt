cmake_minimum_required(VERSION 3.0)
project(glfwPI CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


set(CMAKE_POSITION_INDEPENDENT_CODE 1)

# Make sure we use C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(3RDPARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty")


# Include PictureIt
add_subdirectory("${3RDPARTY_DIR}/PictureIt")
include_directories("${3RDPARTY_DIR}/PictureIt/include")
list(APPEND DEPLIBS PictureIt)


# Include GLFW 
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("${3RDPARTY_DIR}/glfw")
include_directories("${3RDPARTY_DIR}/glfw/include")
list(APPEND DEPLIBS glfw ${GLFW_LIBRARIES})


# Include libsndfile
find_package(libsndfile REQUIRED)
include_directories(${LIBSNDFILE_INCLUDE_DIR})
include_directories(${LIBSNDFILE_INCLUDE_DIRS})
list(APPEND DEPLIBS ${LIBSNDFILE_LIBRARY})


# Include PulseAudio
find_package(PulseAudio REQUIRED)
include_directories(${PULSEAUDIO_INCLUDE_DIR})
list(APPEND DEPLIBS ${PULSEAUDIO_LIBRARY})


set(SOURCES src/main.cpp
            src/audio_player.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${DEPLIBS})


install(TARGETS ${PROJECT_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/bin")
