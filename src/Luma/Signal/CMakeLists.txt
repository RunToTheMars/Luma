cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(ProjLuma_Signal
        VERSION 1.0
        DESCRIPTION "Luma Signal Library"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (LibLuma_Signal
             src/Empty.cpp
             include/Luma/Signal/Signal_fwd.h)

target_include_directories(LibLuma_Signal PUBLIC include)
