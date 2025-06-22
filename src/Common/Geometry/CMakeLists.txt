cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(GLGeometry
        VERSION 1.0
        DESCRIPTION "OpenGL Geometry"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

#include_directories (include)

enable_testing()

# Подключаем GoogleTest (через FetchContent — удобно и просто)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

add_executable(LibCommon_Geometry_Gtests tests/PointTest.cpp)
target_link_libraries(LibCommon_Geometry_Gtests PRIVATE gtest_main)
target_include_directories(LibCommon_Geometry_Gtests PRIVATE include ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include )

# Регистрируем тесты
include(GoogleTest)
gtest_discover_tests(LibCommon_Geometry_Gtests)
