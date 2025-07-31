cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(ProjLumaGLASCII
        VERSION 1.0
        DESCRIPTION "Luma GLASCII"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(LumaGLASCII src/main.cpp)
target_link_libraries(LumaGLASCII
                      PRIVATE libglew_static
                      PRIVATE LibGLASCII
                      PRIVATE LibLuma_Core
                      PRIVATE LibLuma_GL)


