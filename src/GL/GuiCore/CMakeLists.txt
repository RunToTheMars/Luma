cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(GuiCore
        VERSION 1.0
        DESCRIPTION "GuiCore C++ Library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_subdirectory(glfw) # Подключение библиотеки glfw (выполняет CMakeLists, позволяет юзать их target)

add_library (LibGL_GuiCore
             src/Window.cpp
             src/Widget.cpp
             src/Application.cpp
             src/ApplicationPrivate.cpp
             src/Monitor.cpp
             src/Object.cpp
)

target_include_directories(LibGL_GuiCore PUBLIC include)

target_link_libraries(LibGL_GuiCore
                      PRIVATE glfw
                      PUBLIC LibCommon_Geometry
                      PUBLIC LibCommon_Slots)
