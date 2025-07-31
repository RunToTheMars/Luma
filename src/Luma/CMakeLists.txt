cmake_minimum_required(VERSION 3.16)

project(Luma
        VERSION 1.0
        DESCRIPTION "Luma"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(RaiiUtils)
add_subdirectory(Geom)
add_subdirectory(Signal)
add_subdirectory(Core)
add_subdirectory(GL)
