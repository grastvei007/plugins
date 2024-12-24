cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)
project(pluginCore)

include(CheckIncludeFile)
CHECK_INCLUDE_FILE(/opt/vc/include/bcm_host.h BCMHOST)

if(BCMHOST)
    add_definitions(-DRASPBERRY_PI)
endif()


include_directories($ENV{JUNE_ROOT})
link_directories($ENV{DEV_LIBS})

set(CMAKE_AUTOMOC ON)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

add_library(pluginCore SHARED
    plugin.h
    plugin.cpp
    telnet.h
    telnet.cpp
    motor.h motor.cpp
    wiringpiwrapper.h
    wiringpiwrapper.cpp
)

target_link_libraries(pluginCore Qt6::Core)
target_link_libraries(pluginCore Qt6::Network)


if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(pluginCore tagsystemd)
    target_link_libraries(pluginCore pluginLoadd)

    if(BCMHOST)
        target_link_libraries(pluginCore wiringPi)
    endif()
else()
    message("Release mode")
    target_link_libraries(pluginCore tagsystem)
    target_link_libraries(pluginCore pluginLoad)

    if(BCMHOST)
        target_link_libraries(pluginCore wiringPi)
    endif()
endif()

set_target_properties(pluginCore
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


