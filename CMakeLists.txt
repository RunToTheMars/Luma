cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloWorld                      # PROJECT_NAME
        VERSION 1.0                     # PROJECT_VERSION, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH, PROJECT_VERSION_TWEAK
        DESCRIPTION "Try to use OpenGL" # PROJECT_DESCRIPTION
        LANGUAGES CXX)                  # Активирует поддержку C++

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_subdirectory(src/Common/Geometry)
add_subdirectory(src/Common/Signal)
add_subdirectory(src/Common/DoAfterScope)

add_subdirectory(src/GL/glew)
add_subdirectory(src/GL/ASCII)
add_subdirectory(src/GL/ShaderProgram)
add_subdirectory(src/GL/Buffer)
add_subdirectory(src/GL/Widget)

add_subdirectory(src/Targets/HelloASCII)
add_subdirectory(src/Targets/HelloWorld)
