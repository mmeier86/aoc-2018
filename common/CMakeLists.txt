# Common C code for AoC 2018

cmake_minimum_required(VERSION 3.12)
project("Common C Code AoC 2018"
  LANGUAGES C
  )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
string(APPEND CMAKE_C_FLAGS_DEBUG " -Wall -Wextra -Werror")

include(CheckIncludeFile)
function(check_header HEADER)
  check_include_file(${HEADER} HAS_HEADER)
  if(NOT HAS_HEADER)
    message(FATAL_ERROR "Could not file ${HEADER}.\nPlease note that this program uses Linux APIs.")
  endif()
endfunction()
check_header("sys/mman.h")
check_header("sys/stat.h")
check_header("fcntl.h")
check_header("unistd.h")

include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/Unity.cmake)

add_library(aoc_common STATIC common.c)
target_include_directories(aoc_common
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
  )

add_ut(common_ut
  ${CMAKE_CURRENT_LIST_DIR}/test_common.c
  T_DIR ${CMAKE_CURRENT_LIST_DIR}/testfiles
  )
link_ut(common_ut
  PRIVATE aoc_common
)
