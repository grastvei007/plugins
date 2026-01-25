cmake_minimum_required(VERSION 3.28.3)
project(addaboard)

set (CMAKE_CXX_STANDARD 20)
link_directories(${CMAKE_BINARY_DIR}/$<CONFIG>/lib)

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

add_library(addaboard SHARED
    addaboard.h
    addaboard.cpp

    3rdparty/ADS1256.cpp
    3rdparty/ADS1256.h
    3rdparty/Debug.h
    3rdparty/DEV_Config.cpp
    3rdparty/DEV_Config.h
)

target_link_libraries(addaboard Qt6::Core)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(addaboard tagsystemd)
    target_link_libraries(addaboard pluginLoadd)
    target_link_libraries(addaboard pluginCored)

    if(BCMHOST)
        target_link_libraries(addaboard wiringPi)
    endif()
else()
    message("Release mode")
    target_link_libraries(addaboard tagsystem)
    target_link_libraries(addaboard pluginLoad)
    target_link_libraries(addaboard pluginCore)

    if(BCMHOST)
        target_link_libraries(addaboard wiringPi)
    endif()
endif()

set_target_properties(addaboard
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
    )

