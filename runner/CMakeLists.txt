cmake_minimum_required(VERSION 3.16)
project(runner)

link_directories(${CMAKE_BINARY_DIR}/$<CONFIG>/lib)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR "YES")
set(CMAKE_AUTOMOC "YES")
set(CMAKE_AUTOUIC "YES")

find_package(Qt6 COMPONENTS Core REQUIRED)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS HttpServer REQUIRED)

add_executable(runner
    src/main.cpp
    src/app.h
    src/app.cpp
)


target_link_libraries(runner PRIVATE Qt6::Core
    Qt6::HttpServer
    Qt6::Network)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Debug mode")
    target_link_libraries(runner PRIVATE tagsystemd)
    target_link_libraries(runner PRIVATE pluginLoadd)
else()
    message("Release mode")
    target_link_libraries(runner PRIVATE tagsystem)
    target_link_libraries(runner PRIVATE pluginLoad)
endif()

set_target_properties(runner
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)


