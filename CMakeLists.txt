# Minimal settings
cmake_minimum_required(VERSION 3.15)
set(VERSION "0.1.1")
project(C-CNC VERSION ${VERSION} LANGUAGES C)

# Build mode
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  message(NOTICE "Debug build, enabling all warnings")
  add_compile_options(-Wall)
elseif(CMAKE_BUILD_TYPE MATCHES "Release")
  message(STATUS "Release build")
else()
  message(FATAL_ERROR "Please set CMAKE_BUILD_TYPE to either Debug or Release!")
endif()

# Build settings
set(CMAKE_C_STANDARD 11)
set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
set(MAIN_DIR ${SOURCE_DIR}/main)
add_compile_definitions(_GNU_SOURCE)
file(GLOB LIB_SOURCES ${SOURCE_DIR}/*.c)
message(STATUS "Library source files: ${LIB_SOURCES}")

# Configured files
configure_file(${SOURCE_DIR}/defines.h.in ${SOURCE_DIR}/defines.h)

# Target

# static library
add_library(ccnc STATIC ${LIB_SOURCES})

# Test executables
add_executable(point_test ${SOURCE_DIR}/point.c)
target_compile_definitions(point_test PUBLIC POINT_MAIN)
target_link_libraries(point_test m)

add_executable(machine_test ${LIB_SOURCES})
target_compile_definitions(machine_test PUBLIC MACHINE_MAIN)
target_link_libraries(machine_test m)