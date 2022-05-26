cmake_minimum_required(VERSION 3.16)
project(runner)

include_directories($ENV{JUNE_ROOT})
link_directories($ENV{DEV_LIBS})

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)

add_executable(runner
    src/main.cpp
    src/app.h
    src/app.cpp
)


target_link_libraries(runner Qt6::Core)
target_link_libraries(runner Qt6::Network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(runner tagsystemd)
    target_link_libraries(runner pluginLoadd)
else()
    message("Release mode")
    target_link_libraries(runner tagsystem)
    target_link_libraries(runner pluginLoad)
endif()

add_custom_command(TARGET runner
                   POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:runner> $ENV{DEV_BIN} )


