project(cpp_russia VERSION 0.1)

add_executable(cpp_russia main.cpp)
target_link_libraries(cpp_russia
PRIVATE
  nsb_standard_flags
  nsb_compile_options
)

target_sources(cpp_russia
PRIVATE
  1_StorageProxy/StorageProxy.h
  1_StorageProxy/StorageProxy_tests.h
  1_StorageProxy/StorageProxy_tests.cpp
)
