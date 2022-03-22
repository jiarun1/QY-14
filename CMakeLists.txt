cmake_minimum_required(VERSION 3.21)
project(Vehicle)

set(SOURCES
        main.cpp
        Code/Visual.cpp
        Code/Visual.h
        Code/TemplateMatch.cpp Code/TemplateMatch.h)

find_package(OpenCV 4 REQUIRED)

include_directories(
        ${OpenCV_INCLUDE_DIRS}

)


set(CMAKE_CXX_STANDARD 14)

add_executable(Vehicle ${SOURCES})

target_link_libraries(
        Vehicle

        ${OpenCV_LIBS}

)