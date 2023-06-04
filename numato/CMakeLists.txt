cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)
project(Numato)

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

add_library(Numato SHARED
    numato.h
    numato.cpp
)

target_link_libraries(Numato Qt6::Core)

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

add_custom_command(TARGET Numato
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:Numato> $ENV{DEV_LIBS} )


