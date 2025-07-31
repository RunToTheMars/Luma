cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(ASCII
        VERSION 1.0
        DESCRIPTION "OpenGL ASCII C++ Library for Text Render"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (LibShader_ASCII STATIC
             src/Debug/v150/TextLineShader.cpp
             src/Debug/v150/TextBoxShader.cpp)

target_include_directories(LibShader_ASCII PUBLIC include)

target_link_libraries(LibShader_ASCII
                      PRIVATE libglew_static
                      PRIVATE LibShader_Utils)
