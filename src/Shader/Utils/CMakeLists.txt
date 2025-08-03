cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(ShaderUtils
        VERSION 1.0
        DESCRIPTION "OpenGL Shader Utils"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (LibShader_Utils STATIC
             src/Utils.cpp)

target_include_directories(LibShader_Utils PUBLIC include)

target_link_libraries(LibShader_Utils
                      PRIVATE libglew_static)
