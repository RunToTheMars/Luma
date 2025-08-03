cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(BasicShader
        VERSION 1.0
        DESCRIPTION "OpenGL Basic Shader"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (LibShader_Basic STATIC
             src/v110/BasicShader.cpp)

target_include_directories(LibShader_Basic PUBLIC include)

target_link_libraries(LibShader_Basic
                      PRIVATE libglew_static
                      PRIVATE LibShader_Utils)
