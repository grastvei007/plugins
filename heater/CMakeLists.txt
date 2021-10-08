cmake_minimum_required(VERSION 3.16)
project(heater)

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

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

add_library(heater SHARED
    heater.h
    pump.h
    motor.h
    preheatunit.h
    wiringpiwrapper.h

    heater.cpp
    pump.cpp
    motor.cpp
    preheatunit.cpp
    wiringpiwrapper.cpp)

target_link_libraries(heater Qt5::Core)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(heater tagsystemd)
    target_link_libraries(heater pluginLoadd)

    if(BCMHOST)
        target_link_libraries(heater wiringPi)
    endif()
else()
    message("Release mode")
    target_link_libraries(heater tagsystem)
    target_link_libraries(heater pluginLoad)

    if(BCMHOST)
        target_link_libraries(heater wiringPi)
    endif()
endif()

add_custom_command(TARGET heater
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:heater> $ENV{DEV_LIBS} )


