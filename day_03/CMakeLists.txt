# Advent of Code 2018 Day 3

cmake_minimum_required(VERSION 3.12)
project("Advent of Code Day 3"
  LANGUAGES C
  )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
string(APPEND CMAKE_C_FLAGS_DEBUG " -Wall -Wextra -Werror")
include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/Unity.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/common.cmake)

add_library(cloth_cutting
  ${CMAKE_CURRENT_LIST_DIR}/cloth_cutting.c
  )
target_include_directories(cloth_cutting
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
  )
target_link_libraries(cloth_cutting PUBLIC common)

add_executable(day_03
  ${CMAKE_CURRENT_LIST_DIR}/day_03.c
  )
target_link_libraries(day_03
  PUBLIC cloth_cutting
  )

add_ut(test_day_03 test_day_03.c)
link_ut(test_day_03
  PRIVATE cloth_cutting
  )
