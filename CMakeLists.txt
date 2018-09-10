cmake_minimum_required(VERSION 2.8)
project(RG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../run)

set(CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda-8.0)
set(OpenCV_DIR "/usr/local/opencv320/share/OpenCV")
find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB INC *.h)
file(GLOB SRC *.cpp)

add_subdirectory(region_grow)

add_executable( ${PROJECT_NAME} ${INC} ${SRC})

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} region_grow pthread)

add_definitions(-std=c++11)