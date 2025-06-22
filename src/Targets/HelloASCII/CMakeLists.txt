cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloASCII
        VERSION 1.0
        DESCRIPTION "Hello World for ASCII"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(HelloASCII src/main.cpp)
target_link_libraries(HelloASCII
                      PRIVATE libglew_static
                      PRIVATE LibGL_ShaderProgram
                      PRIVATE LibGL_ASCII
                      PRIVATE LibGL_Widget)

