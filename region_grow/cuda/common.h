//
// Created by creator on 18-8-30.
//

#ifndef RG_COMMON_H
#define RG_COMMON_H

#include <opencv2/opencv.hpp>
#include </usr/local/cuda/include/cuda.h>
#include </usr/local/cuda/include/cuda_runtime.h>
#include </usr/local/cuda/include/device_launch_parameters.h>

/** A color in CIELAB colorspace */
struct LAB
{
    /** Lightness */
    float l;
    /** Color-opponent a dimension */
    float a;
    /** Color-opponent b dimension */
    float b;
};

typedef struct LAB LAB;

struct RGB
{
    float r;
    float g;
    float b;
};
typedef struct RGB RGB;

#endif //RG_COMMON_H
