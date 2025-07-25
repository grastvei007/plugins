cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)
project(pigpio)

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
find_package(Qt6 COMPONENTS HttpServer REQUIRED)

add_library(pigpio SHARED
    pigpio.h
    pigpio.cpp
    pin.h
    pin.cpp
    pigpioapi.h
    pigpioapi.cpp
    wiringpiwrapper.h
    wiringpiwrapper.cpp
    pigpio.json
)

target_link_libraries(pigpio Qt6::Core
    Qt6::HttpServer)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(pigpio tagsystemd)
    target_link_libraries(pigpio pluginCored)

    if(BCMHOST)
        target_link_libraries(pigpio wiringPi)
    endif()
else()
    message("Release mode")
    target_link_libraries(pigpio tagsystem)
    target_link_libraries(pigpio pluginCore)

    if(BCMHOST)
        target_link_libraries(pigpio wiringPi)
    endif()
endif()

set_target_properties(pigpio
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


