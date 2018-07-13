cmake_minimum_required(VERSION 3.1)

# Project setup
project(chayai C)

option(USE_PAPI "Use papi to get additional performance data (default OFF)" OFF)

if( USE_PAPI )
    add_definitions( -DUSE_PAPI )
endif()

# Sub projects
add_subdirectory(src)
add_subdirectory(sample)
