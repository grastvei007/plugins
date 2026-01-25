cmake_minimum_required(VERSION 3.16)
project(addaboard)

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

add_library(addaboard SHARED
    addaboard.h
    addaboard.cpp
)

target_link_libraries(addaboard Qt6::Core)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(addaboard tagsystemd)
    target_link_libraries(addaboard pluginLoadd)

    if(BCMHOST)
        target_link_libraries(addaboard wiringPi)
    endif()
else()
    message("Release mode")
    target_link_libraries(addaboard tagsystem)
    target_link_libraries(addaboard pluginLoad)

    if(BCMHOST)
        target_link_libraries(addaboard wiringPi)
    endif()
endif()

add_custom_command(TARGET addaboard
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:addaboard> $ENV{DEV_LIBS} )


