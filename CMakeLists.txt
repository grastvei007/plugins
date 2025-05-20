cmake_minimum_required(VERSION 3.22)
project(JunePlugins)

set (CMAKE_CXX_STANDARD 20)
include_directories($ENV{JUNE_ROOT})
link_directories(${CMAKE_BINARY_DIR}/$<CONFIG>/lib)

add_subdirectory(pluginload)
add_subdirectory(plugincore)
add_subdirectory(runner)

add_subdirectory(bms)
add_subdirectory(heater)
add_subdirectory(victronenergy)
add_subdirectory(ds18b20)
add_subdirectory(pigpio)
add_subdirectory(numato)
