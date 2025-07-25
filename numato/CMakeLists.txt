cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)
project(Numato)

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
find_package(Qt6 REQUIRED COMPONENTS Network)

add_library(Numato SHARED
    numato.h
    numato.cpp
    config.cpp
    util.h
    util.cpp
)

target_link_libraries(Numato Qt6::Core)
target_link_libraries(Numato Qt6::Network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(Numato tagsystemd)
    target_link_libraries(Numato pluginLoadd)
    target_link_libraries(Numato pluginCored)

    if(BCMHOST)
        target_link_libraries(Numato wiringPi)
    endif()
else()
    message("Release mode")
    target_link_libraries(Numato tagsystem)
    target_link_libraries(Numato pluginLoad)
    target_link_libraries(Numato pluginCore)

    if(BCMHOST)
        target_link_libraries(Numato wiringPi)
    endif()
endif()

set_target_properties(Numato
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


