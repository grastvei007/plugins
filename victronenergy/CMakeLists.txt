cmake_minimum_required(VERSION 3.16)
project(victronenergy)

include_directories($ENV{JUNE_ROOT})
link_directories($ENV{DEV_LIBS})

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

add_library(victronenergy SHARED
    victronenergy.h

    victronenergy.cpp)


target_link_libraries(victronenergy Qt6::Core)
target_link_libraries(victronenergy Qt6::Network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(victronenergy tagsystemd)
    target_link_libraries(victronenergy deviced)
    target_link_libraries(victronenergy pluginLoadd)
else()
    message("Release mode")
    target_link_libraries(victronenergy tagsystem)
    target_link_libraries(victronenergy device)
    target_link_libraries(victronenergy pluginLoad)
endif()

add_custom_command(TARGET victronenergy
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:victronenergy> $ENV{DEV_LIBS} )


