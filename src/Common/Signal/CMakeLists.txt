cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(LibCommonSignalGtests
        VERSION 1.0
        DESCRIPTION "Signal"
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (LibCommon_Signal
             src/Empty.cpp)
target_include_directories(LibCommon_Signal PUBLIC include)

enable_testing()

# Подключаем GoogleTest (через FetchContent — удобно и просто)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Тесты
add_executable(LibCommon_Signal_Gtests tests/SignalTest.cpp
  src/Empty.cpp)
target_link_libraries(LibCommon_Signal_Gtests PRIVATE gtest_main)
target_include_directories(LibCommon_Signal_Gtests PRIVATE include ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include )

# Регистрируем тесты
include(GoogleTest)
gtest_discover_tests(LibCommon_Signal_Gtests)
