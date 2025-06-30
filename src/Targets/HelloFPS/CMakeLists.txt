cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloASCII
        VERSION 1.0
        DESCRIPTION "Hello FPS"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(HelloFPSCompat src/main.cpp)
add_executable(HelloFPSCore   src/main.cpp)

target_compile_definitions (HelloFPSCore PRIVATE COREPROFILE)

target_link_libraries(HelloFPSCompat
                      PRIVATE libglew_static
                      PRIVATE LibGL_ASCII
                      PRIVATE LibGL_Widget
                      PRIVATE LibGL_Buffer)

target_link_libraries(HelloFPSCore
                      PRIVATE libglew_static
                      PRIVATE LibGL_ASCII
                      PRIVATE LibGL_Widget
                      PRIVATE LibGL_Buffer
                      PRIVATE LibGL_VertexArray)


