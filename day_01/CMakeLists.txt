# Advent of Code 2018 Day 1

cmake_minimum_required(VERSION 3.12)
project("Advent of Code Day 1"
  LANGUAGES C
  )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
string(APPEND CMAKE_C_FLAGS_DEBUG " -Wall -Wextra -Werror")
set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE)
include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/Unity.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/common.cmake)

check_header("search.h")
check_symbol(tdestroy "search.h")

add_library(chronal_calibration
  ${CMAKE_CURRENT_LIST_DIR}/chronal_calibration.c
  )
target_include_directories(chronal_calibration
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
  )
target_compile_definitions(chronal_calibration
  PRIVATE _GNU_SOURCE
  )
target_link_libraries(chronal_calibration PUBLIC common)

add_executable(day_01
  ${CMAKE_CURRENT_LIST_DIR}/day_01.c
  )
target_link_libraries(day_01
  PUBLIC chronal_calibration
  )

add_ut(test_day_01 test_day_01.c)
link_ut(test_day_01
  PRIVATE chronal_calibration
  )
