cmake_minimum_required(VERSION 3.16)
project(bms)

include_directories($ENV{JUNE_ROOT})
link_directories($ENV{DEV_LIBS})

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core REQUIRED)

add_library(bms SHARED
    bms.cpp
    bms.h)


target_link_libraries(bms Qt5::Core)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(bms tagsystemd)
else()
    message("Release mode")
    target_link_libraries(bms tagsystem)
endif()

