cmake_minimum_required(VERSION 2.8.12)
 
project(dv-decimation-error)

set(ITK_DIR "~/Developer/ITK/bin")
set(VTK_DIR "~/Developer/VTK/bin")

set(CMAKE_CXX_FLAGS "-std=c++11" CACHE STRING "cxx compiler flags" FORCE)
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra" CACHE STRING "cxx debug compiler flags" FORCE)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "build type" FORCE)

include_directories("include")

file(GLOB SOURCES "src/*.cxx")

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(dv-basic-usage dv-basic-usage.cxx)
target_link_libraries(dv-basic-usage ${ITK_LIBRARIES})

add_executable(dv-decimation-error dv-decimation-error.cxx)
target_link_libraries(dv-decimation-error ${ITK_LIBRARIES})

add_executable(dv-wireframe dv-wireframe.cxx ${SOURCES})
target_link_libraries(dv-wireframe ${VTK_LIBRARIES})

