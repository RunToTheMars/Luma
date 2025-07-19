cmake_minimum_required(VERSION 3.16)

project(ProjLumaEvents
        VERSION 1.0
        DESCRIPTION "Luma Events"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(LumaEvents src/main.cpp)
target_link_libraries(LumaEvents
                      PRIVATE LibLuma_Core
                      PRIVATE LibGL_ASCII
                      PRIVATE LibGL_Buffer
                      PRIVATE libglew_static)
