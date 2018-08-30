cmake_minimum_required(VERSION 2.8)
project(RG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../run)

set(OpenCV_DIR "/usr/local/opencv320/share/OpenCV")
find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB INC src/*.hpp)
file(GLOB SRC src/*.cpp)

add_subdirectory(cuda)

add_executable( ${PROJECT_NAME} ${INC} ${SRC})

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} cuda_impl)

add_definitions(-std=c++11)