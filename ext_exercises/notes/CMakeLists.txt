project(notes VERSION 0.0.1)

add_library(notes_lib
STATIC
  Factory/Factory.h
  Factory/Factory.cpp
  Pimpl/Pimpl.h
  Pimpl/Pimpl.cpp
)

target_link_libraries(notes_lib
PRIVATE
  nsb_standard_flags
  nsb_compile_options_for_dev
  libs
)

target_include_directories(notes_lib
PRIVATE
  ${CMAKE_SOURCE_DIR}/libs/
)

add_executable(notes
  "main.cpp"
)

target_link_libraries(notes
PRIVATE
  nsb_standard_flags
  nsb_compile_options_for_dev
  notes_lib
)
