cmake_minimum_required(VERSION 3.16)
project(mcp3008)

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

add_library(mcp3008 SHARED
    mcp3008.h
    wiringpiwrapper.h
    mcp3008.cpp
    wiringpiwrapper.cpp)

target_link_libraries(mcp3008 Qt6::Core)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(mcp3008 tagsystemd)
    target_link_libraries(mcp3008 pluginLoadd)

    if(BCMHOST)
        target_link_libraries(mcp3008 wiringPi)
    endif()
else()
    message("Release mode")
    target_link_libraries(mcp3008 tagsystem)
    target_link_libraries(mcp3008 pluginLoad)

    if(BCMHOST)
        target_link_libraries(mcp3008 wiringPi)
    endif()
endif()

add_custom_command(TARGET mcp3008
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mcp3008> $ENV{DEV_LIBS} )


