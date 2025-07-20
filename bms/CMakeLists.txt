cmake_minimum_required(VERSION 3.16)
project(bms)


if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

add_library(bms SHARED
    bms.h
    bmstags.h

    bms.cpp
    bmstags.cpp)


target_link_libraries(bms Qt6::Core)
target_link_libraries(bms Qt6::Network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(bms tagsystemd)
    target_link_libraries(bms pluginLoadd)
    target_link_libraries(bms pluginCored)
else()
    message("Release mode")
    target_link_libraries(bms tagsystem)
    target_link_libraries(bms pluginLoad)
    target_link_libraries(bms pluginCore)
endif()

set_target_properties(bms
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


