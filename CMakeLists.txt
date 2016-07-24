cmake_minimum_required(VERSION 3.0)
project(glfwPI CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(MSVC)
  add_definitions(-DTARGET_WINDOWS)
else()
  set(CMAKE_POSITION_INDEPENDENT_CODE 1)
  
  # Make sure we use C++11
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF)
endif()


# Include PictureIt
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/depends/PictureIt/add_PictureIt.cmake")
list(APPEND DEPLIBS PictureIt::PictureIt)


# Include GLFW
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/depends/GLFW/add_GLFW.cmake")
include_directories(${GLFW_SRC_DIR}/include)
list(APPEND DEPLIBS glfw ${GLFW_LIBRARIES})


# Include libsndfile
#include(cmake/cmake_helpers/importURLProject.cmake)
#    importURLProject(PROJ                libsndfile
#                     URL "http://www.mega-nerd.com/libsndfile/files"
#                     DOWNLOAD_NAME "libsndfile-1.0.27-w32.zip")
#add_subdirectory(${KissFFT_SOURCE_DIR} ${KissFFT_BINARY_DIR})
#find_package(libsndfile REQUIRED)
#include_directories(${LIBSNDFILE_INCLUDE_DIR})
#include_directories(${LIBSNDFILE_INCLUDE_DIRS})
#list(APPEND DEPLIBS ${LIBSNDFILE_LIBRARY})


# Include PulseAudio
#find_package(PulseAudio REQUIRED)
#include_directories(${PULSEAUDIO_INCLUDE_DIR})
#list(APPEND DEPLIBS ${PULSEAUDIO_LIBRARY})


set(SOURCES src/main.cpp)
            #src/audio_player.cpp)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${DEPLIBS})


install(TARGETS ${PROJECT_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/bin")
