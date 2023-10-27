cmake_minimum_required(VERSION 3.10)
project(gravity_sim_cpp)

# Include Directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# OpenCV Setup
set(OpenCV_DIR C:/Users/guill/Documents/opencv/OpenCV-MinGW-Build-OpenCV-4.5.5-x64)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Source Files
file(GLOB_RECURSE SOURCES 
     src/main.cpp
     src/glad.c
     src/engine/*.cpp
     src/game/*.cpp
     src/states/*.cpp)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "app")


# Link Directories and Libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${CMAKE_SOURCE_DIR}/lib/libglfw3dll.a)





