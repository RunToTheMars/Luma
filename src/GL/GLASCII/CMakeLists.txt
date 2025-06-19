cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(GLASCII
        VERSION 1.0
        DESCRIPTION "OpenGL ASCII C++ Library for Text Render"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (libgl_ascii STATIC
             src/impl/v130/Word.cpp
             src/impl/v150/Word.cpp)

target_include_directories(libgl_ascii PUBLIC include)

target_link_libraries(libgl_ascii
                      PRIVATE libglew_static
                      PRIVATE libgl_shaderprogram)
