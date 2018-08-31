set(MODULE_NAME region_grow)

find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB INC *.h)
file(GLOB SRC *.cpp)

add_subdirectory(cuda)

add_library( ${MODULE_NAME} ${INC} ${SRC})

target_link_libraries( ${MODULE_NAME} ${OpenCV_LIBS} cuda_region_grow)

add_definitions(-std=c++11)