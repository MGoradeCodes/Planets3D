
cmake_minimum_required(VERSION 3.20)

project(Planets3D LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(libs/glfw)

add_executable(Planets3D
    src/main.cpp
    libs/glad/src/gl.c
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
    src/ShaderClass.cpp
    src/Camera.cpp
    src/Mesh.cpp
    src/stb.cpp
    src/Texture.cpp
)

target_compile_definitions(Planets3D PRIVATE GLM_ENABLE_EXPERIMENTAL)
target_include_directories(Planets3D PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/glad/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/libs/glm"
)

target_link_libraries(Planets3D PRIVATE
    glfw
    opengl32
)

set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/shaders")
set(TEXTURE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/textures")

add_custom_command(TARGET Planets3D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SHADER_SOURCE_DIR}"
        "$<TARGET_FILE_DIR:Planets3D>/shaders"

    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${TEXTURE_SOURCE_DIR}"
        "$<TARGET_FILE_DIR:Planets3D>/textures"

    COMMENT "Copying shaders and textures..."
    VERBATIM
)
