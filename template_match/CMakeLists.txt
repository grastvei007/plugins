cmake_minimum_required(VERSION 3.16)
set (CMAKE_CXX_STANDARD 20)
project(template_match)

include(CheckIncludeFile)
CHECK_INCLUDE_FILE(/opt/vc/include/bcm_host.h BCMHOST)

if(BCMHOST)
    add_definitions(-DRASPBERRY_PI)
endif()

set(CMAKE_AUTOMOC ON)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

SET("OpenCV_DIR" "/usr/share/OpenCV")
find_package( OpenCV REQUIRED )

MESSAGE(STATUS "Include dirs ${OpenCV_INCLUDE_DIRS}")
MESSAGE(STATUS "LINK LIBRARIES ${OpenCV_LIBS}")

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Gui)

add_library(template_match SHARED
    template_match.h
    template_match.cpp
    )


target_link_libraries(template_match Qt6::Core Qt6::Gui)
target_link_libraries(template_match ${OpenCV_LIBS})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(template_match tagsystemd)
    target_link_libraries(template_match pluginCored)

    if(BCMHOST)
        target_link_libraries(template_match wiringPi)
    endif()
else()
    message("Release mode")
    target_link_libraries(template_match tagsystem)
    target_link_libraries(template_match pluginCore)

    if(BCMHOST)
        target_link_libraries(template_match wiringPi)
    endif()
endif()

set_target_properties(template_match
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


