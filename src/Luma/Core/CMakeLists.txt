cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(ProjLuma_Core
        VERSION 1.0
        DESCRIPTION "Luma Core Library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_subdirectory(glfw) # Подключение библиотеки glfw (выполняет CMakeLists, позволяет юзать их target)

add_library (LibLuma_Core
             src/Object.cpp
             src/Application.cpp
             src/Window.cpp
             src/Monitor.cpp
             include/Luma/Core/ScaleEvent.h)

target_include_directories(LibLuma_Core PUBLIC include)

target_link_libraries(LibLuma_Core
                      PRIVATE glfw
                      PUBLIC LibLuma_Signal
                      PUBLIC LibLuma_Geom)
