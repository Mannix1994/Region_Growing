
set(MODULE_NAME cuda_impl)

set(CU_INC cide.h common.h)
set(CU_SRC cide.cpp cide.cu)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
find_package(CUDA 8 REQUIRED)
CUDA_SELECT_NVCC_ARCH_FLAGS(code_generation_flags "${CUDA_ARCHITECTURES}")
list(APPEND CUDA_NVCC_FLAGS ${code_generation_flags})
message(STATUS "CUDA_NVCC_FLAGS=${code_generation_flags}")

set(CMAKE_CXX_FLAGS "-std=c++11")

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
