project(libs VERSION 0.1)

add_library(libs STATIC
  NsbVector/NsbVector.h
  NsbVector/NsbVector.cpp
  utility.h
)
target_link_libraries(libs
PRIVATE
  nsb_standard_flags
  nsb_compile_options
)

add_executable(libs_testing
"libs_testing/main.cpp"
"libs_testing/utility_test.h"
)
target_include_directories(libs_testing
PRIVATE
  "./"
)
target_link_libraries(libs_testing
PRIVATE
  nsb_standard_flags
  nsb_compile_options
  libs
  gtest
)