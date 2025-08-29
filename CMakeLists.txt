cmake_minimum_required(VERSION 3.31.5)
project("nsb-project" VERSION 0.1 DESCRIPTION "Not a simple sandbox")

configure_file(mainConfig.h.in mainConfig.h @ONLY)

add_library(nsb_compile_options INTERFACE)
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(nsb_compile_options INTERFACE
#"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Werror;-Wpedantic;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_library(nsb_compile_options_for_dev INTERFACE)
target_compile_options(nsb_compile_options_for_dev INTERFACE
"$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wpedantic;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
"$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)

add_library(nsb_standard_flags INTERFACE)
target_compile_features(nsb_standard_flags INTERFACE cxx_std_23)
#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_CXX_STANDARD_REQUIRED True)
# set_property(TARGET app CXX_STANDARD 23)

# External libraries:
set(EXT_LIBS "ext_libs")
set(EXT_LIBS_PREFIX "")
if (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  if (MSVC)
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "notes")
    set(IS_DEBUG True)
    if (${IS_DEBUG})
      set(CMAKE_CONFIGURATION_TYPES "Debug;")
      set(EXT_LIBS_PREFIX "Debug")
    else ()
      set(CMAKE_CONFIGURATION_TYPES "Release;")
      set(EXT_LIBS_PREFIX "Release")
    endif ()
  endif ()
endif ()

include(CTest)
include(GoogleTest)
enable_testing()

find_package(GTest 1.17.0 EXACT REQUIRED
PATHS "${CMAKE_SOURCE_DIR}/${EXT_LIBS}/gtest_1_17_0/${EXT_LIBS_PREFIX}"
)
message(STATUS "GTest ${GTest_VERSION} [ ${GTest_DIR} ]")

add_library(nsb_ext_gtest INTERFACE)
target_link_libraries(nsb_ext_gtest
INTERFACE
  GTest::gtest
  GTest::gtest_main
)

find_package(Boost 1.88.0 EXACT REQUIRED
COMPONENTS
  program_options
PATHS "${CMAKE_SOURCE_DIR}/${EXT_LIBS}/boost_1_88_0/${EXT_LIBS_PREFIX}"
)
message(STATUS "BOOST ${Boost_VERSION} [ ${GTest_DIR} ]")

add_library(nsb_ext_boost INTERFACE)
target_include_directories(nsb_ext_boost INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(nsb_ext_boost
INTERFACE 
  Boost::program_options
)

add_library(nsb_ext_all_libs INTERFACE)
target_link_libraries(nsb_ext_all_libs
INTERFACE
  nsb_ext_gtest
  nsb_ext_boost
)
# ~External libraries

add_subdirectory(app)
add_subdirectory(libs)
add_subdirectory(notes)

# External exercises
add_subdirectory(ext_exercises)
# ~External exercises

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
