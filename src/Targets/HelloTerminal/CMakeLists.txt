cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloTerminal
        VERSION 1.0
        DESCRIPTION "Hello Terminal"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(HelloTerminal src/main.cpp)
target_link_libraries(HelloTerminal
                      PRIVATE libglew_static
                      PRIVATE LibGL_ASCII
                      PRIVATE LibGL_Widget
                      PRIVATE LibGL_Buffer
                      PRIVATE LibGL_VertexArray)


