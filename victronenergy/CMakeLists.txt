cmake_minimum_required(VERSION 3.16)
project(victronenergy)

include_directories($ENV{JUNE_ROOT})
link_directories($ENV{DEV_LIBS})

set(CMAKE_AUTOMOC ON)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

add_library(victronenergy SHARED
    ../plugincore/plugin.h
    ../pluginload/plugininterface.h
    victronenergy.h
    victronenergy.cpp)


target_link_libraries(victronenergy Qt6::Core)
target_link_libraries(victronenergy Qt6::Network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(victronenergy tagsystemd)
    target_link_libraries(victronenergy deviced)
    target_link_libraries(victronenergy pluginLoadd)
    target_link_libraries(victronenergy pluginCored)
else()
    message("Release mode")
    target_link_libraries(victronenergy tagsystem)
    target_link_libraries(victronenergy device)
    target_link_libraries(victronenergy pluginLoad)
    target_link_libraries(victronenergy pluginCore)
endif()

set_target_properties(victronenergy
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


