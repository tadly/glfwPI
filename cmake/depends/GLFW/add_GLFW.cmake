set(GLFW_SRC_DIR ${CMAKE_BINARY_DIR}/3rdParty/GLFW-src)
set(GLFW_BUILD_DIR ${CMAKE_BINARY_DIR}/3rdParty/GLFW-build)

# Download and unpack GLFW at configure time
configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeLists.txt.in
               ${GLFW_BUILD_DIR}/CMakeLists.txt)

execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                WORKING_DIRECTORY ${GLFW_BUILD_DIR})

execute_process(COMMAND ${CMAKE_COMMAND} --build .
                WORKING_DIRECTORY ${GLFW_BUILD_DIR})


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


add_subdirectory(${GLFW_SRC_DIR}
                 ${GLFW_BUILD_DIR})


mark_as_advanced(FORCE ${GLFW_SRC_DIR} ${GLFW_BUILD_DIR})
