cmake_minimum_required(VERSION 3.16)

project(ProjLuma_GL
        VERSION 1.0
        DESCRIPTION "Luma OpenGL Library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library (LibLuma_GL
             src/Buffer.cpp
             src/ShaderProgram.cpp
             src/VertexArray.cpp)

target_include_directories(LibLuma_GL
                           PUBLIC include)

target_link_libraries(LibLuma_GL
                      PRIVATE libglew_static)
