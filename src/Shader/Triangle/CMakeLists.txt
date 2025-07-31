cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(GLASCII
        VERSION 1.0
        DESCRIPTION "OpenGL ASCII C++ Library for Text Render"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (LibShader_Triangle STATIC
             src/v150/TriangleShader.cpp)

target_include_directories(LibShader_Triangle PUBLIC include)

target_link_libraries(LibShader_Triangle
                      PRIVATE libglew_static
                      PRIVATE LibShader_Utils)
