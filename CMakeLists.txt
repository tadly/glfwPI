cmake_minimum_required(VERSION 3.0)
project(glfwPI CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


find_package(CXX11 REQUIRED)


set(DEPENDENCIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dependencies")


# Use existing PictureIt installation
#find_package(PictureIt REQUIRED)


# Build PictureIt alongside glfwPI 
add_subdirectory("${DEPENDENCIES_DIR}/PictureIt")
include_directories("${DEPENDENCIES_DIR}/PictureIt/include")
list(APPEND DEPLIBS PictureIt)


# Build GLFW alongside glfwPI
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory("${DEPENDENCIES_DIR}/glfw")
include_directories("${DEPENDENCIES_DIR}/glfw/include")
list(APPEND DEPLIBS glfw ${GLFW_LIBRARIES})


add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${DEPLIBS})


install(TARGETS ${PROJECT_NAME} DESTINATION "${PROJECT_SOURCE_DIR}/bin")
