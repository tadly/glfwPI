set(GLFW_TARGET ${CMAKE_BINARY_DIR}/3rdParty)

# Download and unpack GLFW at configure time
configure_file(${CMAKE_CURRENT_LIST_DIR}/GLFW-CMakeLists.txt.in
               ${GLFW_TARGET}/GLFW-download/CMakeLists.txt)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${GLFW_TARGET}/GLFW-download )

execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${GLFW_TARGET}/GLFW-download )

# Add GLFW directly to our build.
add_subdirectory(${GLFW_TARGET}/GLFW-src
                 ${GLFW_TARGET}/GLFW-build)