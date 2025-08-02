project(effective_cpp VERSION 0.1)

add_executable(effective_cpp main.cpp
unit_1.h
)
target_link_libraries(effective_cpp
PRIVATE
  nsb_standard_flags
  nsb_ext_all_libs
  # nsb_compile_options
)
