cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloWorld                      # PROJECT_NAME
        VERSION 1.0                     # PROJECT_VERSION, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH, PROJECT_VERSION_TWEAK
        DESCRIPTION "Try to use OpenGL" # PROJECT_DESCRIPTION
        LANGUAGES CXX)                  # Активирует поддержку C++

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

find_package (Qt6 REQUIRED COMPONENTS CORE)

add_subdirectory(src/GL/glew)
add_subdirectory(src/GL/GLASCII)
add_subdirectory(src/GL/GLShaderProgram)
add_subdirectory(src/GL/GLWindow)
add_subdirectory(src/GL/Geometry)

add_executable(hello_world src/HelloWorld/main.cpp)
target_link_libraries(hello_world
                      PRIVATE libglew_static
                      PRIVATE libgl_window)

add_executable(hello_ascii src/HelloASCII/main.cpp
                      src/GL/Geometry/include/Point.h
                      src/GL/Geometry/include/Size.h)
target_link_libraries(hello_ascii
                      PRIVATE libglew_static
                      PRIVATE libgl_shaderprogram
                      PRIVATE libgl_ascii
                      PRIVATE libgl_window)
