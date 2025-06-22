cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloWorld
        VERSION 1.0
        DESCRIPTION "Hello World for OpenGL"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(HelloWorld src/main.cpp)
target_link_libraries(HelloWorld
                      PRIVATE libglew_static
                      PRIVATE LibGL_Window)
