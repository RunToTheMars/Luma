cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(HelloWorld                      # PROJECT_NAME
        VERSION 1.0                     # PROJECT_VERSION, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH, PROJECT_VERSION_TWEAK
        DESCRIPTION "Try to use OpenGL" # PROJECT_DESCRIPTION
        LANGUAGES CXX)                  # Активирует поддержку C++

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_subdirectory(src/glew)
add_subdirectory(src/GLASCII)

add_subdirectory(src/Luma/RaiiUtils)
add_subdirectory(src/Luma/Geom)
add_subdirectory(src/Luma/Signal)
add_subdirectory(src/Luma/Core)
add_subdirectory(src/Luma/GL)

#add_subdirectory(src/Targets/HelloWindowFPS)
#add_subdirectory(src/Targets/HelloTriangle)
#add_subdirectory(src/Targets/HelloTerminal)

add_subdirectory(src/Targets/LumaWindow)
add_subdirectory(src/Targets/LumaEvents)
add_subdirectory(src/Targets/LumaGLASCII)
add_subdirectory(src/Targets/LumaTerminal)
