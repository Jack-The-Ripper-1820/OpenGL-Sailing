# CMakeList.txt : CMake project for OpenGL-Sailing, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("OpenGL-Sailing")

# Set the source directory
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)


# Add source to this project's executable.
add_executable (OpenGL-Sailing
    "${SRC_DIR}/main.cpp"
)

# Include directories for dependencies
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/assimp)
add_subdirectory(dependencies/glew)


target_include_directories(OpenGL-Sailing PRIVATE
    "${SRC_DIR}"
    "${CMAKE_SOURCE_DIR}/dependencies/glfw/include"
    "${CMAKE_SOURCE_DIR}/dependencies/glm"
    "${CMAKE_SOURCE_DIR}/dependencies/assimp/include"
    "${CMAKE_SOURCE_DIR}/dependencies/glew/include" 

)

target_link_libraries(
    OpenGL-Sailing
    glfw
    glm
    assimp
    libglew_static
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OpenGL-Sailing PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
