cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(GLFWCPP
        VERSION 1.0
        DESCRIPTION "GLFW C++ Library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_subdirectory(glfw) # Подключение библиотеки glfw (выполняет CMakeLists, позволяет юзать их target)

add_library (libgl_window
             src/Window.cpp
             include/GL/Widget.h
             src/Widget.cpp

     )

target_include_directories(libgl_window PUBLIC include)

target_link_libraries(libgl_window
                      PRIVATE glfw)
