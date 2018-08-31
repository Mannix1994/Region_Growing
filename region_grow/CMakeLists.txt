set(MODULE_NAME region_grow)

find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB INC *.h *.hpp)
file(GLOB SRC *.cpp)

add_library( ${MODULE_NAME} SHARED ${INC} ${SRC})

target_link_libraries( ${MODULE_NAME} ${OpenCV_LIBS})

add_definitions(-std=c++11)