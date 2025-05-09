
project(concurrency_libs VERSION 0.0.1)

add_library(concurrency_libs STATIC
  concurrency/JThread.h
  concurrency/ThreadGuard.h
  concurrency/ThreadGuard.cpp
)
target_link_libraries(concurrency_libs
PRIVATE
  nsb_standard_flags
  nsb_compile_options_for_dev
)

add_executable(concurrency_cpp
  "main.cpp"
)
target_include_directories(concurrency_cpp
PRIVATE
  "./concurrency/"
)
target_link_libraries(concurrency_cpp
PRIVATE
  nsb_standard_flags
  nsb_compile_options_for_dev
  concurrency_libs
)
