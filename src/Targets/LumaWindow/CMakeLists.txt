cmake_minimum_required(VERSION 3.16)

project(ProjLumaWindow
        VERSION 1.0
        DESCRIPTION "Luma Window"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LumaWindow src/main.cpp)

target_link_libraries(LumaWindow
                      PRIVATE LibLuma_Core)
