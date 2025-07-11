cmake_minimum_required(VERSION 3.16)  # Минимальная версия cmake = 3.16

project(LibCommonSlotsGtests
        VERSION 1.0
        DESCRIPTION "Slots"
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)          # Версия стандарта С++
set(CMAKE_CXX_STANDARD_REQUIRED ON) # Обязательно ли использовать именно этот стандарт, или можно выше.

add_library (LibCommon_Slots
             src/Empty.cpp)
target_include_directories(LibCommon_Slots PUBLIC include)

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
add_executable(LibCommon_Slots_Gtests tests/SignalTest.cpp
  src/Empty.cpp)
target_link_libraries(LibCommon_Slots_Gtests PRIVATE gtest_main)
target_include_directories(LibCommon_Slots_Gtests PRIVATE include ${gtest_SOURCE_DIR} ${gtest_SOURCE_DIR}/include )

# Регистрируем тесты
include(GoogleTest)
gtest_discover_tests(LibCommon_Slots_Gtests)
