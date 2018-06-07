cmake_minimum_required(VERSION 2.6)

# Project setup
project(c-hayai C)

option(USE_PAPI "Use papi to get additional performance data (default OFF)" OFF)

if( USE_PAPI )
    add_definitions( -DUSE_PAPI )
endif()

# Sub projects
add_subdirectory(src)
add_subdirectory(sample)
