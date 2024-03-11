cmake_minimum_required(VERSION 3.10)
project(gravity_sim_cpp)


# Variables pour les chemins des dépendances
if(WIN32)
    #OpenCV
    set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/lib/OpenCV/include")
    
set(OpenCV_LIBS
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_calib3d455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_core455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_dnn455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_features2d455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_flann455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_gapi455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_highgui455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_imgcodecs455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_imgproc455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_ml455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_objdetect455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_photo455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_stitching455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_video455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/libopencv_videoio455.dll"
    "${CMAKE_SOURCE_DIR}/lib/OpenCV/bin/opencv_videoio_ffmpeg455_64.dll"
    "${CMAKE_SOURCE_DIR}/lib/SDL2/SDL2.dll"
    "${CMAKE_SOURCE_DIR}/lib/SDL2/SDL2.dll"
    "${CMAKE_SOURCE_DIR}/lib/GNU/libmpdec++-2.dll"
    "${CMAKE_SOURCE_DIR}/lib/GNU/libgcc_s_seh-1.dll"
    "${CMAKE_SOURCE_DIR}/lib/GNU/libstdc++-6.dll"
    "${CMAKE_SOURCE_DIR}/lib/GNU/libwinpthread-1.dll"
    "${CMAKE_SOURCE_DIR}/lib/Steam/steam_api64.dll"
    




)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -v")


    #GLM
    include_directories("${CMAKE_SOURCE_DIR}/lib/glm")
    
    #SLD2
    set(SDL2_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/SDL2/SDL2.dll") # This is where headers are expected
    set(SDL2_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/lib/SDL2/include") # This is where the SDL2 library is expected

    #SLD_MIXER
    set(SDL2_MIXER_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/lib/SLD2_mixer/include") # Mettre à jour avec le bon chemin
    set(SDL2_MIXER_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/SLD2_mixer/SDL2_mixer.dll")       # Mettre à jour avec le bon chemin

    #steam
    set(STEAM_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/lib/Steam/steam") # Mettre à jour avec le bon chemin
    set(STEAM_LIB_DIR "${CMAKE_SOURCE_DIR}/lib/Steam/")
    find_library(STEAM_LIB NAMES steam_api64 PATHS ${STEAM_LIB_DIR})

    #CURL
    set(CURL_DIRS "${CMAKE_SOURCE_DIR}/lib/CURL/include") # Mettre à jour avec le bon chemin
    set(CURL_LIBRARIES "${CMAKE_SOURCE_DIR}/lib/CURL/libcurl-x64.dll")       # Mettre à jour avec le bon chemin

    set(GCC_STATIC_LIBRARIES "-static-libgcc -static-libstdc++")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_STATIC_LIBRARIES}")
    
    include_directories(${CMAKE_SOURCE_DIR}/include/GLFW)
    include_directories(${CURL_DIRS})
    include_directories(${OpenCV_DIR})

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


# Include Directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui)
include_directories(${CMAKE_SOURCE_DIR}/external/imgui/backends)
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
include_directories(${STEAM_INCLUDE_DIRS})
message(STATUS "Steam include directory: ${STEAM_INCLUDE_DIRS}")

# Source Files
file(GLOB_RECURSE SOURCES 
     src/main.cpp
     src/glad.c
     src/engine/*.cpp
     src/engine/RenderTools/*.cpp
     src/engine/Planets/*.cpp
     src/engine/PhysicsTools/*.cpp
     src/engine/CreationTools/*.cpp
     src/engine/QuizTools/*.cpp
     src/engine/SuccessTools/*.cpp
     src/save/*.cpp
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
# Executable
# Executable
# Configurez le fichier .icns pour qu'il soit copié dans le répertoire de build
if(WIN32)
  # Append the .rc file for Windows build
  list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/assets/icons/ressources.rc")
endif()

if(APPLE)
set(APP_ICON ${CMAKE_CURRENT_SOURCE_DIR}/assets/icons/myIcon.icns)
configure_file(${APP_ICON} ${CMAKE_CURRENT_BINARY_DIR}/myIcon.icns COPYONLY)

# Définir les propriétés du fichier icône pour qu'il soit inclus dans le bundle
set_source_files_properties(${APP_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

# Ajoutez l'exécutable avec l'icône comme ressource
add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES} ${APP_ICON})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Space Query")


    # Configuration supplémentaire pour le bundle macOS
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION "1.0"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in
    )
elseif(WIN32)
add_executable(${PROJECT_NAME}  ${SOURCES})
########add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "Space Query")

set(ALL_DLLS 
    ${OpenCV_LIBS}
    "${CMAKE_SOURCE_DIR}/lib/CURL/libcurl-x64.dll"
    "${CMAKE_SOURCE_DIR}/lib/GLFW/glfw3.dll"
    "${CMAKE_SOURCE_DIR}/lib/SLD2_mixer/SDL2_mixer.dll"
    # Add any other DLLs here
)
foreach(DLL ${ALL_DLLS})
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${DLL} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endforeach()
endif()
#set_target_properties(${PROJECT_NAME} PROPERTIES 
#    WIN32_EXECUTABLE TRUE
#)
# Link Directories and Libraries
link_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${GLFW_LIB} ${SDL2_MIXER_LIBRARIES} ${SDL2_LIBRARIES} ${CURL_LIBRARIES} ${CMAKE_SOURCE_DIR}/lib/GLFW/glfw3.dll ${OpenCV_LIBS} ${STEAM_LIB})



# Configuration de base de CPack
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "1")
set(CPACK_PACKAGE_VERSION_PATCH "2")

# Spécification du générateur (par exemple, NSIS sous Windows)
if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_MODIFY_PATH ON)
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop")
endif()

include(CPack)

# Installer l'exécutable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets DESTINATION .)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/engine/RenderTools/Shaders DESTINATION src/engine/RenderTools)
# Installer les bibliothèques nécessaires et les fichiers de ressources
install(FILES ${ALL_DLLS} DESTINATION bin)

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_MODIFY_PATH ON)

    # Ajout de commandes pour créer un raccourci sur le bureau
    set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
        CreateShortCut \\\"$DESKTOP\\\\Gravity Simulator.lnk\\\" \\\"$INSTDIR\\\\bin\\\\${PROJECT_NAME}.exe\\\"
    ")
endif()