cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloWindowEvents
        VERSION 1.0
        DESCRIPTION "Hello Window Events"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(HelloWindowEvents src/main.cpp)
target_link_libraries(HelloWindowEvents
                      PRIVATE LibGL_GuiCore
                      PRIVATE libglew_static)
