cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(GLEWCPP
        VERSION 1.0
        DESCRIPTION "GLEW C++ Library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (LibGL_ShaderProgram
             src/ShaderProgram.cpp
     )

target_include_directories(LibGL_ShaderProgram PUBLIC include)

target_link_libraries(LibGL_ShaderProgram
                      PRIVATE libglew_static)
