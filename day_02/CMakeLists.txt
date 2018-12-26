# Advent of Code 2018 Day 2

cmake_minimum_required(VERSION 3.12)
project("Advent of Code Day 2"
  LANGUAGES C
  )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
string(APPEND CMAKE_C_FLAGS_DEBUG " -Wall -Wextra -Werror")
include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/Unity.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/common.cmake)

add_library(inventory_mgmt
  ${CMAKE_CURRENT_LIST_DIR}/inventory_mgmt.c
  )
target_include_directories(inventory_mgmt
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
  )
target_link_libraries(inventory_mgmt PUBLIC common)

add_executable(day_02
  ${CMAKE_CURRENT_LIST_DIR}/day_02.c
  )
target_link_libraries(day_02
  PUBLIC inventory_mgmt
  )

add_ut(test_day_02 test_day_02.c)
link_ut(test_day_02
  PRIVATE inventory_mgmt
  )
