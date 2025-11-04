cmake_minimum_required(VERSION 3.29.2)

project(DynMaker VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(UNIX)
  message("This is a ${CMAKE_SYSTEM_NAME} system")
  add_definitions(-Dcmakemacro="Unix")
elseif(WIN32)
  message("This is windows")
  add_definitions(-Dcmakemacro="Windows")
endif()

set(DISPLAY_SERVER "windows" CACHE STRING "Display manager in linux: x11 or wayland")
message("Display Server is : ${DISPLAY_SERVER}")
add_definitions(-DDISPLAY_SERVER="${DISPLAY_SERVER}")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)


######################################################
###################  THE  ############################
############     Actual Convertor   ##################
######################################################
######################################################

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(
  SOURCES 
  ${SOURCE_DIR}/Mathril.cpp
  ${SOURCE_DIR}/dynmain.cpp
  ${SOURCE_DIR}/util.cpp
) 

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
set(
  HEADER_FILES 
  ${INC_DIR}/Mathril.h
  ${INC_DIR}/json.hpp
  ${INC_DIR}/util.h
)
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADER_FILES})
set(INCS ${INC_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCS})



######################################################
####################    THE   ########################
###############      Dyn Viewer     ##################
######################################################
######################################################

set(VIEWER_PROJ viewDyn)

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(
  VIEWER_SOURCES 
  ${SOURCE_DIR}/Mathril.cpp
  ${SOURCE_DIR}/Camera.cpp
  ${SOURCE_DIR}/Light.cpp
  ${SOURCE_DIR}/Mesh.cpp
  ${SOURCE_DIR}/MeshAnimated.cpp
  ${SOURCE_DIR}/ObjectAnimated.cpp
  ${SOURCE_DIR}/ObjectRendererAnimated.cpp
  ${SOURCE_DIR}/glad.c
  ${SOURCE_DIR}/Projection.cpp
  ${SOURCE_DIR}/ShaderProgram.cpp
  ${SOURCE_DIR}/Texture.cpp
  ${SOURCE_DIR}/Win.cpp
  ${SOURCE_DIR}/viewerMain.cpp
  ${SOURCE_DIR}/Engine.cpp
) 

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
set(
  VIEWER_HEADER_FILES 
  ${INC_DIR}/Mathril.h
  ${INC_DIR}/Camera.h
  ${INC_DIR}/Light.h
  ${INC_DIR}/Mesh.h
  ${INC_DIR}/MeshAnimated.h
  ${INC_DIR}/ObjectAnimated.h
  ${INC_DIR}/ObjectRendererAnimated.h
  ${INC_DIR}/Projection.h
  ${INC_DIR}/ShaderProgram.h
  ${INC_DIR}/Texture.h
  ${INC_DIR}/Win.h
  ${INC_DIR}/Engine.h
)
if(WIN32)
  set(
    VIEWER_HEADER_FILES 
    ${VIEWER_HEADER_FILES}
    ${INC_DIR}/GLFW/glfw3.h
    ${INC_DIR}/GLFW/glfw3native.h
  )
endif()
add_executable(${VIEWER_PROJ} ${VIEWER_SOURCES} ${VIEWER_HEADER_FILES})


#OpenGL
find_package(OpenGL REQUIRED)
if(WIN32)
  #GLFW: NOTE: find_package does not works for libraries that are not installed 
  set(GLFW_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include/GLFW")
  include_directories(${GLFW_INCLUDE_DIRS})
  if(MINGW)
    link_directories(${CMAKE_SOURCE_DIR}/library/minGW)
    set(GLFW_LIBRARIES "${CMAKE_SOURCE_DIR}/library/minGW/glfw3.lib")
  else()
    link_directories(${CMAKE_SOURCE_DIR}/library/msvc)
    set(GLFW_LIBRARIES "${CMAKE_SOURCE_DIR}/library/msvc/glfw3.lib")
  endif()
  # Put all libraries into a variable
  set(LIBS ${GLFW_LIBRARIES} ${OPENGL_LIBRARY})
  # Put all includes into a variable
  set(INCS ${INC_DIR} ${GLFW_INCLUDE_DIRS})
elseif(UNIX)
  find_package(glfw3 REQUIRED)
  set(LIBS glfw ${OPENGL_LIBRARY})
  set(INCS ${INC_DIR} ${GLFW_INCLUDE_DIRS})
  if(${DISPLAY_SERVER} MATCHES "wayland")
    find_library(LIB_WAYLAND_CURSOR
      NAMES libwayland-cursor libwayland-cursor.so wayland-cursor 
      )
    find_library(LIB_DL
      NAMES libdl libdl.so 
      )
    set(LIBS ${LIBS} ${LIB_WAYLAND_CURSOR} ${LIB_DL})
  elseif(${DISPLAY_SERVER} MATCHES "x11")
    find_package(x11 REQUIRED)
    set(LIBS ${LIBS} ${X11_LIBRARIES})
    set(INCS ${INCS} ${X11_INCLUDE_DIR})
  endif()
endif()

# Define the link libraries
target_link_libraries(${VIEWER_PROJ} ${LIBS})

target_include_directories(${VIEWER_PROJ} PUBLIC ${INCS})

# Copy Directories to bin
add_custom_target(copy-runtime-files ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/bin/shaders
    DEPENDS ${VIEWER_PROJ})


# use following command

## windows MinGW
# mkdir build 
# cd build
# cmake -G "MinGW Makefiles" -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++ -DDISPLAY_SERVER=windows ..
# cd bin
# cmake --build ..

## windows MVSC (default) use the `developer terminal` 
# mkdir build
# cd build
# cmake -G "Visual Studio 17 2022" -A x64 -DDISPLAY_SERVER=windows ..
# cmake --build . (notice no cd bin this time for some reason)

## linux wayland
# mkdir build
# cd build
# cmake -G "Unix Makefiles" -DDISPLAY_SERVER=wayland .. 
# cd bin
# cmake --build ..
