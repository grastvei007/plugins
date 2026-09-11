cmake_minimum_required(VERSION 3.16)
project(nve)


if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

add_library(nve SHARED
    nve.h
    nve.cpp
)


target_link_libraries(nve Qt6::Core)
target_link_libraries(nve Qt6::Network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(nve tagsystemd)
    target_link_libraries(nve pluginLoadd)
    target_link_libraries(nve pluginCored)
else()
    message("Release mode")
    target_link_libraries(nve tagsystem)
    target_link_libraries(nve pluginLoad)
    target_link_libraries(nve pluginCore)
endif()

set_target_properties(nve
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


