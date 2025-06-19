cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(GLEWCPP
        VERSION 1.0
        DESCRIPTION "GLEW C++ Library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (libgl_shaderprogram
             src/ShaderProgram.cpp
             src/Shader.cpp)

target_include_directories(libgl_shaderprogram PUBLIC include)

target_link_libraries(libgl_shaderprogram
                      PRIVATE libglew_static)
