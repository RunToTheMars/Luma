cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloWindow
        VERSION 1.0
        DESCRIPTION "Hello World"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(HelloWindow src/main.cpp)
target_link_libraries(HelloWindow
                      PRIVATE LibGL_GuiCore)
