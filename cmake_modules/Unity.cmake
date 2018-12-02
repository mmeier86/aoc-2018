##
# Definitions for Using Unity UT FW
##

option(UNITTESTS_ENABLED
  "ENABLE Unit Test Functionality"
  OFF
  )
if(UNITTESTS_ENABLED)
  enable_testing()
  add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} -V
    )

  set(UNITY_DIR ${CMAKE_CURRENT_LIST_DIR}/../third_party/unity)

  add_library(unity STATIC
    ${UNITY_DIR}/src/unity.c
    )
  target_include_directories(unity
    PUBLIC
    ${UNITY_DIR}/src
    )
  target_compile_definitions(unity
    PUBLIC
    UNITY_OUTPUT_COLOR
    UNITY_INCLUDE_DOUBLE
    )
  add_library(unity::unity ALIAS unity)

  macro(add_ut t_name t_file)
    cmake_parse_arguments(ADDUT "" "T_DIR" "" ${ARGN})
    add_executable(${t_name} ${t_file})
    target_link_libraries(${t_name} PRIVATE unity::unity)
    add_test(NAME ${t_name}
      COMMAND ${t_name} ${ADDUT_T_DIR}
      )
    add_dependencies(check ${t_name})
  endmacro()
  macro(link_ut t_name)
    target_link_libraries(${t_name} ${ARGN})
  endmacro()
else()
  macro(add_ut)
  endmacro()
  macro(link_ut)
  endmacro()
endif()
