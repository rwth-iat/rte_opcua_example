cmake_minimum_required(VERSION 3.13)

project(rte_opcua_example C)

# Disable default build of unrequired libraries
SET(BUILD_OV_LIBRARY_cshmi OFF CACHE BOOL "")

# Include ov core
add_subdirectory(rte)

# Include ov helper functions and definitions
get_target_property(RTE_CMAKE_DIR ov RTE_CMAKE_DIR)
include(${RTE_CMAKE_DIR}/ov_functions.cmake)
include(${RTE_CMAKE_DIR}/ov_definitions.cmake)

# Include libraries of this project
# Example:
add_subdirectory(opcuaExample)

ov_finish_project()
