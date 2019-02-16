# Advent of Code 2018 Day 3

cmake_minimum_required(VERSION 3.12)
project("Advent of Code Day 4"
  LANGUAGES C
  )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
string(APPEND CMAKE_C_FLAGS_DEBUG " -Wall -Wextra -Werror")
set(CMAKE_REQUIRED_DEFINITIONS -D_XOPEN_SOURCE -D_GNU_SOURCE)
include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/Unity.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/common.cmake)

check_header(regex.h)
include(CheckSymbolExists)
check_symbol_exists(strptime "time.h" HAS_STRPTIME)
if(NOT HAS_STRPTIME)
  message(FATAL_ERROR "Could not find \"strptime\" function. Please note that this program needs the GNU extensions enabled.")
endif()
check_header(search.h)
check_symbol_exists(tdestroy "search.h" HAS_TDESTROY)
if(NOT HAS_TDESTROY)
  message(FATAL_ERROR "Could not find \"tdestroy\" function. Please note that this program needs the GNU extensions enabled.")
endif()

add_library(repose_record
  ${CMAKE_CURRENT_LIST_DIR}/repose_record.c
  )
target_include_directories(repose_record
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
  )
target_link_libraries(repose_record PUBLIC common)
target_compile_definitions(repose_record PUBLIC
  -D_XOPEN_SOURCE
  -D_GNU_SOURCE
  )

add_executable(day_04
  ${CMAKE_CURRENT_LIST_DIR}/day_04.c
  )
target_link_libraries(day_04
  PUBLIC repose_record
  )

add_ut(test_day_04 test_day_04.c)
link_ut(test_day_04
  PRIVATE repose_record
  )
