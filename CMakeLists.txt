cmake_minimum_required(VERSION 3.16)
project(JunePlugins)

set (CMAKE_CXX_STANDARD 20)
include_directories($ENV{JUNE_ROOT})

add_subdirectory(runner)

add_subdirectory(pluginload)
add_subdirectory(bms)
add_subdirectory(heater)
add_subdirectory(victronenergy)
add_subdirectory(ds18b20)
add_subdirectory(pigpio)


