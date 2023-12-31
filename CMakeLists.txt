cmake_minimum_required(VERSION 3.10)
project(gravity_sim_cpp)

# Variables pour les chemins des dépendances
if(WIN32)
    set(OpenCV_DIR "C:/Users/guill/Documents/opencv/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
    set(GLFW_LIB "${CMAKE_SOURCE_DIR}/lib/libglfw3dll.a")
    # Assume SDL2 is installed in a standard location on Windows or specify its path
    find_package(SDL2 REQUIRED)
    elseif(APPLE)
    # Ajustez le chemin pour OpenCV sur macOS si nécessaire.
    set(GLFW_LIB "/opt/homebrew/lib/libglfw.3.3.dylib")
    
    # Manually set SDL2 paths
    set(SDL2_INCLUDE_DIRS "/opt/homebrew/include/SDL2") # This is where headers are expected
    set(SDL2_LIBRARIES "/opt/homebrew/lib/libSDL2.dylib") # This is where the SDL2 library is expected
    
    # Manually set SDL2_mixer paths
    set(SDL2_MIXER_INCLUDE_DIRS "/opt/homebrew/include/SDL2") # This is where headers are expected
    set(SDL2_MIXER_LIBRARIES "/opt/homebrew/lib/libSDL2_mixer.dylib") # This is where the library is expected
    include_directories("/opt/homebrew/Cellar/glm/0.9.9.8/include")
    include_directories("/opt/homebrew/Cellar/opencv/4.8.1_4/include/opencv4")
    include_directories("/opt/homebrew/Cellar/glew/2.2.0_1/include")
    find_package(CURL REQUIRED)
    include_directories(${CURL_INCLUDE_DIRS})

endif()

# Configuration OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})


# Include Directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/backends)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_MIXER_INCLUDE_DIRS})

# Source Files
file(GLOB_RECURSE SOURCES 
     src/main.cpp
     src/glad.c
     src/engine/*.cpp
     src/engine/RenderTools/*.cpp
     src/engine/Planets/*.cpp
     src/engine/PhysicsTools/*.cpp
     src/engine/CreationTools/*.cpp
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
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Space Querry")

# Link Directories and Libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${GLFW_LIB} ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${CURL_LIBRARIES})
