project(libs VERSION 0.1)

add_library(libs STATIC
  caches/caches.h
  caches/caches.hpp
  NsbVector/NsbVector.h
  NsbVector/NsbVector.cpp
  utility.h
)
target_link_libraries(libs
PRIVATE
  nsb_standard_flags
  nsb_compile_options
  # nsb_compile_options_for_dev
)

add_executable(libs_testing
  "libs_testing/caches/caches_test.cpp"
  "libs_testing/main.cpp"
  "libs_testing/utility_test.cpp"
)
target_include_directories(libs_testing
PRIVATE
  "./"
  "./caches/"
)
target_link_libraries(libs_testing
PRIVATE
  nsb_standard_flags
  nsb_compile_options
  # nsb_compile_options_for_dev
  libs
  gtest
)