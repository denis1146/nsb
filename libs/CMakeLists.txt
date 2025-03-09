project(libs VERSION 0.1)
#add_library(foo INTERFACE)
#set(with_variadics ${CMAKE_CURRENT_SOURCE_DIR}/with_variadics)
#set(no_variadics ${CMAKE_CURRENT_SOURCE_DIR}/no_variadics)
#target_include_directories(foo
#  INTERFACE
#    "$<$<COMPILE_FEATURES:cxx_variadic_templates>:${with_variadics}>"
#    "$<$<NOT:$<COMPILE_FEATURES:cxx_variadic_templates>>:${no_variadics}>"
#  )

add_library(libs STATIC
  NsbVector/NsbVector.h
  NsbVector/NsbVector.cpp
)
target_link_libraries(libs
PRIVATE
  nsb_standard_flags
  nsb_compile_options
)

add_executable(libs_tests main.cpp)
target_link_libraries(libs_tests
PRIVATE
  libs
)