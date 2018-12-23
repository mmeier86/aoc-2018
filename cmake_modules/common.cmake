# Include module for AoC common utilities

include(ExternalProject)

set(COMMON_DIR ${CMAKE_CURRENT_LIST_DIR}/../common)

ExternalProject_Add(common-ext
  SOURCE_DIR ${COMMON_DIR}
  CMAKE_ARGS -DUNITTESTS_ENABLED=OFF -DCMAKE_BUILD_TYPE=Release
  CMAKE_GENERATOR ${CMAKE_GENERATOR}
  BUILD_BYPRODUCTS <BINARY_DIR>/libaoc_common.a
  INSTALL_COMMAND ""
  )

ExternalProject_Get_Property(common-ext binary_dir)
add_library(common-imp STATIC IMPORTED)
set_property(TARGET common-imp
  PROPERTY
    IMPORTED_LOCATION ${binary_dir}/libaoc_common.a
  )
add_dependencies(common-imp common-ext)

add_library(common INTERFACE)
target_link_libraries(common INTERFACE common-imp)
target_include_directories(common INTERFACE
  ${COMMON_DIR}
  )
