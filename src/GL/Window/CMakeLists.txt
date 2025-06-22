cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(GLFWCPP
        VERSION 1.0
        DESCRIPTION "GLFW C++ Library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_subdirectory(glfw) # Подключение библиотеки glfw (выполняет CMakeLists, позволяет юзать их target)

add_library (LibGL_Window
             src/Window.cpp
     )

target_include_directories(LibGL_Window PUBLIC include)

target_link_libraries(LibGL_Window
                      PRIVATE glfw)
