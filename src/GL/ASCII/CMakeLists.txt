cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(GLASCII
        VERSION 1.0
        DESCRIPTION "OpenGL ASCII C++ Library for Text Render"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (LibGL_ASCII STATIC
             src/v130/Word.cpp
             src/v130_2/Word.cpp
             src/v150/Word.cpp)

target_include_directories(LibGL_ASCII PUBLIC include)

target_link_libraries(LibGL_ASCII
                      PRIVATE libglew_static
                      PUBLIC  LibGL_ShaderProgram
                      PRIVATE LibCommon_DoAfterScope)
