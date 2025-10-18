cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)
project(ds18b20)

include(CheckIncludeFile)
CHECK_INCLUDE_FILE(/opt/vc/include/bcm_host.h BCMHOST)

if(BCMHOST)
    add_definitions(-DRASPBERRY_PI)
endif()

set(CMAKE_AUTOMOC ON)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

add_library(ds18b20 SHARED
    ds18b20.h
    ds18b20.cpp)

target_link_libraries(ds18b20 Qt6::Core)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(ds18b20 tagsystemd)
    target_link_libraries(ds18b20 pluginCored)

    if(BCMHOST)
        target_link_libraries(ds18b20 wiringPi)
    endif()
else()
    message("Release mode")
    target_link_libraries(ds18b20 tagsystem)
    target_link_libraries(ds18b20 pluginCore)

    if(BCMHOST)
        target_link_libraries(ds18b20 wiringPi)
    endif()
endif()

set_target_properties(ds18b20
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


