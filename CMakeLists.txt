cmake_minimum_required(VERSION 3.31.5)
project("nsb-project" VERSION 0.1 DESCRIPTION "Not a simple sandbox")

configure_file(mainConfig.h.in mainConfig.h @ONLY)

#set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo")

add_library(nsb_compile_options INTERFACE)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
#[[
target_compile_options(nsb_compile_options INTERFACE
 "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
 "$<${msvc_cxx}:-W3>"
)
]]
target_compile_options(nsb_compile_options INTERFACE
#"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Werror;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_library(nsb_standard_flags INTERFACE)
target_compile_features(nsb_standard_flags INTERFACE cxx_std_23)
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED True)
# set_property(TARGET app CXX_STANDARD 23)

add_subdirectory(app)
add_subdirectory(cpp_russia)
add_subdirectory(effective_cpp)
add_subdirectory(libs)

#[[
message(STATUS "=== Print system information:")
include(CMakePrintSystemInformation)
message(STATUS "========================")

message(STATUS "=== Print system information:")
include(CMakePrintHelpers)
cmake_print_variables(PROJECT_NAME CMAKE_PROJECT_NAME)
cmake_print_variables(CMAKE_BUILD_TYPE)
cmake_print_properties(TARGETS nsb_compile_options app PROPERTIES BINARY_DIR)
cmake_print_properties(TARGETS app nsb_compile_options PROPERTIES
  COMPILE_DEFINITIONS COMPILE_FEATURES COMPILE_OPTIONS COMPILE_FLAGS 
  INTERFACE_COMPILE_DEFINITIONS INTERFACE_COMPILE_FEATURES INTERFACE_COMPILE_OPTIONS)
cmake_print_variables(CMAKE_C_COMPILER CMAKE_CXX_COMPILER)
cmake_print_variables(CMAKE_C_STANDARD CMAKE_CXX_STANDARD)
message(STATUS "========================")
]]
