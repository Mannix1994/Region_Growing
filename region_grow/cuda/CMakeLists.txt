
set(MODULE_NAME cuda_region_grow)

set(CU_INC cide.h common.h)
set(CU_SRC cide.cpp cide.cu)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
find_package(CUDA 8 REQUIRED)

CUDA_SELECT_NVCC_ARCH_FLAGS(CUDA_NVCC_FLAGS Auto)
list(APPEND CUDA_NVCC_FLAGS -O2 -std=c++11)
message(STATUS "CUDA_NVCC_FLAGS=${CUDA_NVCC_FLAGS}")

set(CMAKE_CXX_FLAGS "-std=c++11 -O3")

include_directories(${CUDA_INCLUDE_DIRS})


cuda_add_library(${MODULE_NAME} SHARED
        ${CU_INC}
        ${CU_SRC}
        )

set_target_properties(${MODULE_NAME}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
        CXX_VISIBILITY_PRESET hidden
        )

#use cublas
CUDA_ADD_CUBLAS_TO_TARGET(${MODULE_NAME})
