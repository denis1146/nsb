project(notes VERSION 0.0.1)

add_library(notes_lib
STATIC
  CountingInString/CountingInString.h
  CountingInString/CountingInString.cpp
  Boost/Boost.h
  Boost/Boost.cpp
  Boost/Spirit/Spirit.h
  Boost/Spirit/Spirit.cpp
  Boost/ProgramOptions/ProgramOptions.h
  Boost/ProgramOptions/ProgramOptions.cpp
  Factory/Factory.h
  Factory/Factory.cpp
  Pimpl/Pimpl.h
  Pimpl/Pimpl.cpp
  Ranges/RangesTest.h
  Ranges/RangesTest.cpp
  ScopeExit/ScopeExit.h
  ScopeExit/ScopeExit.cpp
  SimpleAny/SimpleAny.h
  SimpleAny/SimpleAny.cpp
  TemplateTest/TemplateTest.h
  TemplateTest/TemplateTest.cpp
  Transformations/Transformations.h
  Transformations/Transformations.cpp
  FindAndReplace/FindAndReplace.h
  FindAndReplace/FindAndReplace.cpp
  NoteHelpers.h
)

target_link_libraries(notes_lib
PRIVATE
  nsb_standard_flags
  nsb_compile_options_for_dev
  nsb_ext_boost
  libs
)

target_include_directories(notes_lib
PRIVATE
  ./
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
