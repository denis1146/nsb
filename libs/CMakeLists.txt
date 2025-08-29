project(libs VERSION 0.1)

add_library(libs STATIC
  caches/caches.h
  caches/caches.hpp
  containers/Array.h
  containers/Array.hpp
  containers/iterator.h
  NsbVector/NsbVector.h
  NsbVector/NsbVector.cpp
  numeric/reduce.h
  numeric/reduce.hpp
  predef.h
  type_traits.h
  utility.h
)
target_link_libraries(libs
PRIVATE
  nsb_standard_flags
  nsb_compile_options
  # nsb_compile_options_for_dev
)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
target_link_libraries(libs
PRIVATE
  tbb
)
endif ()

target_include_directories(libs
PRIVATE
  "./"
)

add_executable(libs_testing
  "libs_testing/caches/caches_test.cpp"
  "libs_testing/containers/Array_test.cpp"
  "libs_testing/containers/iterator_test.cpp"
  "libs_testing/numeric/reduce_test.cpp"
  "libs_testing/main.cpp"
  "libs_testing/predef_test.cpp"
  "libs_testing/type_traits_test.cpp"
  "libs_testing/utility_test.cpp"
)
target_include_directories(libs_testing
PRIVATE
  "./"
  "./caches/"
  "./containers/"
)
target_link_libraries(libs_testing
PRIVATE
  nsb_standard_flags
  nsb_compile_options
  # nsb_compile_options_for_dev
  nsb_ext_gtest
  nsb_ext_boost
  libs
)

gtest_discover_tests(libs_testing
  # TEST_FILTER "utility*"
)

add_test(NAME ALL_TESTS
  COMMAND libs_testing
)
