cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloTriangle
        VERSION 1.0
        DESCRIPTION "Hello Triangle"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(2_HelloTriangleCompat src/main.cpp)
add_executable(2_HelloTriangleCore   src/main.cpp)

target_compile_definitions (2_HelloTriangleCore PRIVATE COREPROFILE)

target_link_libraries(2_HelloTriangleCompat
                      PRIVATE libglew_static
                      PRIVATE LibGL_Widget
                      PRIVATE LibGL_Buffer
                      PRIVATE LibGL_ShaderProgram)

target_link_libraries(2_HelloTriangleCore
                      PRIVATE libglew_static
                      PRIVATE LibGL_Widget
                      PRIVATE LibGL_Buffer
                      PRIVATE LibGL_ShaderProgram
                      PRIVATE LibGL_VertexArray)


