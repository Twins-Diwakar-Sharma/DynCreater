cmake_minimum_required(VERSION 3.29.2)

project(StcMaker VERSION 1.0)
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

set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/source)
set(
  SOURCES 
  ${SOURCE_DIR}/Algebra.cpp
  ${SOURCE_DIR}/GltfLoader.cpp
  ${SOURCE_DIR}/Loader.cpp
  ${SOURCE_DIR}/MIndexTree.cpp
  ${SOURCE_DIR}/stcMaker.cpp
) 

set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
set(
  HEADER_FILES 
  ${INC_DIR}/Algebra.h
  ${INC_DIR}/GltfLoader.h
  ${INC_DIR}/Loader.h
  ${INC_DIR}/MIndexTree.h
  ${INC_DIR}/json.hpp
)


# can use this to pass config settings from cmake to source tree
# configure_file(${INC_DIR}/Config.h.in ${INC_DIR}/Config.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADER_FILES})

# generate compile_commands.json or .ccls for ccls 
if(UNIX OR MINGW) # Others (MSVC) does not generate this json
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_SOURCE_DIR}/compile_commands.json
  )
else()
  file(WRITE ${CMAKE_SOURCE_DIR}/.ccls "clang++\n-I${CMAKE_SOURCE_DIR}/include")
endif()

set(INCS ${INC_DIR})

target_include_directories(${PROJECT_NAME} PUBLIC ${INCS})


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
