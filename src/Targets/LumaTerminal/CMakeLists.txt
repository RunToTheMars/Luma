cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(ProjLumaTerminal
        VERSION 1.0
        DESCRIPTION "Luma Terminal"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_executable(LumaTerminal src/main.cpp
                            src/Terminal.cpp
                            src/TerminalImpl.inl)

target_include_directories(LumaTerminal PUBLIC include)

target_link_libraries(LumaTerminal
                      PRIVATE libglew_static
                      PRIVATE LibGLASCII
                      PRIVATE LibLuma_Core
                      PRIVATE LibLuma_GL)


