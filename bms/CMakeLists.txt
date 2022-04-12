cmake_minimum_required(VERSION 3.16)
project(bms)

include_directories($ENV{JUNE_ROOT})
link_directories($ENV{DEV_LIBS})

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
else()
    message("Release mode")
    target_link_libraries(bms tagsystem)
    target_link_libraries(bms pluginLoad)
endif()

add_custom_command(TARGET bms
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:bms> $ENV{DEV_LIBS} )


