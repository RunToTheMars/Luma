cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(Shaders
        VERSION 1.0
        DESCRIPTION "OpenGL Shaders"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_subdirectory(Utils)
add_subdirectory(ASCII)
add_subdirectory(Basic)
