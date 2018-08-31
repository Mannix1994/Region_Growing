cmake_minimum_required(VERSION 2.8)
project(RG)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../run)

set(OpenCV_DIR "/usr/local/opencv320/share/OpenCV")
find_package(OpenCV 3 REQUIRED)

file(GLOB INC region_grow/*.hpp *.h)
file(GLOB SRC region_grow/*.cpp *.cpp)

add_executable( ${PROJECT_NAME} ${INC} ${SRC})

target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )

add_definitions(-std=c++11)