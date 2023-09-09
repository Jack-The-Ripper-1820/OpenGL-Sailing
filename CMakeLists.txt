# CMakeList.txt : CMake project for OpenGL-Sailing, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

#set(CMAKE_TOOLCHAIN_FILE "C:/src/vcpkg/scripts/buildsystems/vcpkg.cmake")

project ("OpenGL-Sailing")

# Set the source directory
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

# Add source files from the common folder
file(GLOB COMMON_SRC
    "${SRC_DIR}/common/*.*"
)

file(GLOB SHADER_FILES "${SRC_DIR}/shaders/*.*")

file(GLOB MODEL_FILES "${SRC_DIR}/models/*.*")

# Recursive function to find all files in a directory and its subdirectories
function(copy_all_files sourceDir destinationDir)
    file(GLOB children RELATIVE ${sourceDir} ${sourceDir}/*)
    foreach(child ${children})
        if(IS_DIRECTORY ${sourceDir}/${child})
            # Create a corresponding directory in the destination folder
            file(MAKE_DIRECTORY ${destinationDir}/${child})
            copy_all_files(${sourceDir}/${child} ${destinationDir}/${child})
        else()
            # Copy the file to the destination folder
            configure_file(${sourceDir}/${child} ${destinationDir}/${child} COPYONLY)
        endif()
    endforeach()
endfunction()

# Copy all texture files and directories recursively to the build directory
copy_all_files("${SRC_DIR}/textures" "${CMAKE_BINARY_DIR}/textures")

# Add source to this project's executable.
add_executable (OpenGL-Sailing
    "${SRC_DIR}/main.cpp"
    ${COMMON_SRC}
    ${SHADER_FILES}
)

# Include directories for dependencies
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/assimp)
add_subdirectory(dependencies/glew)

foreach(SHADER_FILE ${SHADER_FILES})
    get_filename_component(SHADER_FILENAME ${SHADER_FILE} NAME)
    configure_file(${SHADER_FILE} ${CMAKE_BINARY_DIR}/shaders/${SHADER_FILENAME} COPYONLY)
endforeach()

foreach(MODEL_FILE ${MODEL_FILES})
    get_filename_component(MODEL_FILENAME ${MODEL_FILE} NAME)
    configure_file(${MODEL_FILE} ${CMAKE_BINARY_DIR}/models/${MODEL_FILENAME} COPYONLY)
endforeach()

target_include_directories(OpenGL-Sailing PUBLIC
    "${SRC_DIR}"
    "${CMAKE_BINARY_DIR}/shaders"
    "${CMAKE_SOURCE_DIR}/dependencies/glfw/include"
    "${CMAKE_SOURCE_DIR}/dependencies/glm"
    "${CMAKE_SOURCE_DIR}/dependencies/assimp/include"
    "${CMAKE_SOURCE_DIR}/dependencies/glew/include" 
    "${CMAKE_SOURCE_DIR}/dependencies/stb_image"
    "${SRC_DIR}/common" 
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
