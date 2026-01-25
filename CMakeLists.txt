cmake_minimum_required(VERSION 3.22)
project(JunePlugins)

set (CMAKE_CXX_STANDARD 20)
link_directories(${CMAKE_BINARY_DIR}/$<CONFIG>/lib)

include(CheckIncludeFile)
CHECK_INCLUDE_FILE(/opt/vc/include/bcm_host.h BCMHOST)

add_subdirectory(pluginload)
add_subdirectory(plugincore)
add_subdirectory(runner)

add_subdirectory(bms)
add_subdirectory(heater)
add_subdirectory(victronenergy)
add_subdirectory(ds18b20)
add_subdirectory(pigpio)
add_subdirectory(numato)
add_subdirectory(addaboard)
# plugin that make no sense to build and run on a
# raspberry pi
if(NOT BCMHOST)
    add_subdirectory(template_match)
endif()

add_dependencies(pluginLoad tagsystem)
add_dependencies(pluginCore tagsystem pluginLoad)
add_dependencies(runner tagsystem pluginLoad)

add_dependencies(bms tagsystem pluginCore)
add_dependencies(heater tagsystem pluginCore)
add_dependencies(victronenergy tagsystem pluginCore)
add_dependencies(ds18b20 tagsystem pluginCore)
add_dependencies(pigpio tagsystem pluginCore)
add_dependencies(Numato tagsystem pluginCore)
add_dependencies(addaboard tagsystem pluginLoad pluginCore)
if(NOT BCMHOST)
    add_dependencies(template_match tagsystem pluginCore)
endif()
