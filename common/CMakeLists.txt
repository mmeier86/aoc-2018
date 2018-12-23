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
include(CheckSymbolExists)
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

check_symbol_exists(strtok_r "string.h" HAS_STRTOK)
if(NOT HAS_STRTOK)
  message(FATAL_ERROR "string.h does not provide strtok_r.\n\
Please note that this program uses Linux APIs."
    )
endif()

if(UNITTESTS_ENABLED)
  check_symbol_exists(open_memstream "stdio.h" HAS_MEMSTREAM)
  if(NOT HAS_MEMSTREAM)
    message(FATAL_ERROR "stdio.h does not provide open_memstream.\n\
Please note that this program uses Linux APIs."
      )
  endif()
endif()

include(${CMAKE_CURRENT_LIST_DIR}/../cmake_modules/Unity.cmake)

add_library(aoc_common STATIC
  mm_files.c
  tokenizer.c
  main.c
  )
target_include_directories(aoc_common
  PUBLIC ${CMAKE_CURRENT_LIST_DIR}
  )

add_ut(mm_files_ut
  ${CMAKE_CURRENT_LIST_DIR}/test_mm_files.c
  ${CMAKE_CURRENT_LIST_DIR}/mm_files.c
  T_DIR ${CMAKE_CURRENT_LIST_DIR}/testfiles_mm_files
  )
add_ut(tokenizer_ut
  ${CMAKE_CURRENT_LIST_DIR}/test_tokenizer.c
  ${CMAKE_CURRENT_LIST_DIR}/tokenizer.c
  )
add_ut(main_ut
  ${CMAKE_CURRENT_LIST_DIR}/test_main.c
  ${CMAKE_CURRENT_LIST_DIR}/main.c
  ${CMAKE_CURRENT_LIST_DIR}/common_mocks.c
  )
add_ut(main_integration_ut
  ${CMAKE_CURRENT_LIST_DIR}/test_main_integration.c
  ${CMAKE_CURRENT_LIST_DIR}/main.c
  ${CMAKE_CURRENT_LIST_DIR}/mm_files.c
  ${CMAKE_CURRENT_LIST_DIR}/tokenizer.c
  T_DIR ${CMAKE_CURRENT_LIST_DIR}/testfiles_main_integration
  )
