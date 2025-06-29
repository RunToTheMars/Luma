cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(1_HelloWorld
        VERSION 1.0
        DESCRIPTION "Hello World"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(1_HelloWorld src/main.cpp)
target_link_libraries(1_HelloWorld
                      PRIVATE libglew_static
                      PRIVATE LibGL_Widget)
