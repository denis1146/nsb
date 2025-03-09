project(app VERSION 0.1)

add_executable(app main.cpp)
target_link_libraries(app
PRIVATE
  nsb_standard_flags
  nsb_compile_options
)
target_include_directories(app PRIVATE ${CMAKE_BINARY_DIR})
target_sources(app PRIVATE ${CMAKE_BINARY_DIR}/mainConfig.h)
target_compile_definitions(app PRIVATE CMAKE_CXX_COMPILER="${CMAKE_CXX_COMPILER}")
