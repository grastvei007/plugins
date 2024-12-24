cmake_minimum_required(VERSION 3.16)
project(JunePluginLoader)


include_directories($ENV{JUNE_ROOT})
link_directories($ENV{DEV_LIBS})

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_library(pluginLoad SHARED
    pluginloader.h
    plugininterface.h

    pluginloader.cpp
    )


target_link_libraries(pluginLoad Qt6::Core)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(pluginLoad tagsystemd)
else()
    message("Release mode")
    target_link_libraries(pluginLoad tagsystem)
endif()


set_target_properties(pluginLoad
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)
