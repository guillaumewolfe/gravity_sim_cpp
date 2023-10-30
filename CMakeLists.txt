cmake_minimum_required(VERSION 3.10)
project(gravity_sim_cpp)

# Variables pour les chemins des dépendances
if(WIN32)
    set(OpenCV_DIR "C:/Users/guill/Documents/opencv/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
    set(GLFW_LIB "${CMAKE_SOURCE_DIR}/lib/libglfw3dll.a")
elseif(APPLE)
    # Ajustez le chemin pour OpenCV sur macOS si nécessaire.
    set(GLFW_LIB "/opt/homebrew/lib/libglfw.3.3.dylib")
endif()

# Configuration OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Include Directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/backends)

# Source Files
# Source Files
file(GLOB_RECURSE SOURCES 
     src/main.cpp
     src/glad.c
     src/engine/*.cpp
     src/game/*.cpp
     src/UI/*.cpp
     src/states/*.cpp
     external/imgui/imgui.cpp
     external/imgui/imgui_draw.cpp
     external/imgui/imgui_widgets.cpp
     external/imgui/imgui_demo.cpp
     external/imgui/imgui_tables.cpp
     external/imgui/backends/imgui_impl_glfw.cpp
     external/imgui/backends/imgui_impl_opengl3.cpp)

# Executable
add_executable(${PROJECT_NAME} ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "app")

# Link Directories and Libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${GLFW_LIB})
