//
// Created by creator on 18-8-30.
//

#include "cide.h"
#include "common.h"

#ifndef M_PI
# define M_PI        3.14159265358979323846f    /* pi */
#endif
/**
 * turn bgr color c to xyc color
 * @param bgr a bgr color
 * @return a xyc color
 */
__device__ float3 toXYC(uchar3 bgr) {

    float xyz[3];
    float r = (bgr.z / 255.0f);//channel R
    float g = (bgr.y / 255.0f);//channel G
    float b = (bgr.x / 255.0f);//channel B

    if (r > 0.04045)
        r = (float) pow(((r + 0.055) / 1.055), 2.4);
    else
        r = r / 12.92f;

    if (g > 0.04045)
        g = (float) pow(((g + 0.055) / 1.055), 2.4);
    else
        g = g / 12.92f;

    if (b > 0.04045)
        b = (float) pow(((b + 0.055) / 1.055), 2.4);
    else
        b = b / 12.92f;

    r *= 100;
    g *= 100;
    b *= 100;

    //Observer. = 2°, Illuminant = D65
    xyz[0] = r * 0.4124f + g * 0.3576f + b * 0.1805f;
    xyz[1] = r * 0.2126f + g * 0.7152f + b * 0.0722f;
    xyz[2] = r * 0.0193f + g * 0.1192f + b * 0.9505f;

    float3 va;
    va.x = xyz[0];//channel X
    va.y = xyz[1];//channel Y
    va.z = xyz[2];//channel Z
    return va;
}

/**
 * turn bgr color to a lab color
 * @param bgr a bgr color
 * @return a lab color
 */
__device__ LAB toLAB(uchar3 bgr) {
    float3 xyz = toXYC(bgr);
    float lab[3];
    float x = xyz.x / 95.047f; //Observer. = 2°, Illuminant = D65
    float y = xyz.y / 100.000f;
    float z = xyz.z / 108.883f;

    if (x > 0.008856)
        x = (float) pow(x, (1.0 / 3.0));
    else
        x = (7.787f * x) + (16 / 116);

    if (y > 0.008856)
        y = (float) pow(y, (1.0 / 3.0));
    else
        y = (7.787f * y) + (16 / 116);

    if (z > 0.008856)
        z = (float) pow(z, (1.0 / 3.0));
    else
        z = (7.787f * z) + (16 / 116);

    lab[0] = (116 * y) - 16;
    lab[1] = 500 * (x - y);
    lab[2] = 200 * (y - z);

    lab[0] = lab[0] < 0.0 ? 0.0f : lab[0];

    LAB va;
    va.l = lab[0];//channel L
    va.a = lab[1];//channel A
    va.b = lab[2];//channel B
    return va;
}

__device__ float deg2Rad(const float deg) {
    return (deg * (M_PI / 180.0f));
}

__device__ float cide_distance(uchar3 bgr1, uchar3 bgr2) {
    LAB lab1 = toLAB(bgr1);
    LAB lab2 = toLAB(bgr2);

    const float k_L = 1.0, k_C = 1.0, k_H = 1.0;
    const float deg360InRad = deg2Rad(360.0);
    const float deg180InRad = deg2Rad(180.0);
    const float pow25To7 = 6103515625.0f; /* pow(25, 7) */

    /*
	 * Step 1
	 */
    /* Equation 2 */
    float C1 = (float) sqrt((lab1.a * lab1.a) + (lab1.b * lab1.b));
    float C2 = (float) sqrt((lab2.a * lab2.a) + (lab2.b * lab2.b));
    /* Equation 3 */
    float barC = (C1 + C2) / 2.0f;
    /* Equation 4 */
    float G = 0.5f * (float) (1 - sqrt(pow(barC, 7) / (pow(barC, 7) + pow25To7)));
    /* Equation 5 */
    float a1Prime = (1.0f + G) * lab1.a;
    float a2Prime = (1.0f + G) * lab2.a;
    /* Equation 6 */
    float CPrime1 = (float) sqrt((a1Prime * a1Prime) + (lab1.b * lab1.b));
    float CPrime2 = (float) sqrt((a2Prime * a2Prime) + (lab2.b * lab2.b));
    /* Equation 7 */
    float hPrime1;
    if (lab1.b == 0 && a1Prime == 0)
        hPrime1 = 0.0;
    else {
        hPrime1 = (float) atan2(lab1.b, a1Prime);
        /*
         * This must be converted to a hue angle in degrees between 0
         * and 360 by addition of 2􏰏 to negative hue angles.
         */
        if (hPrime1 < 0)
            hPrime1 += deg360InRad;
    }
    float hPrime2;
    if (lab2.b == 0 && a2Prime == 0)
        hPrime2 = 0.0;
    else {
        hPrime2 = (float) atan2(lab2.b, a2Prime);
        /*
         * This must be converted to a hue angle in degrees between 0
         * and 360 by addition of 2􏰏 to negative hue angles.
         */
        if (hPrime2 < 0)
            hPrime2 += deg360InRad;
    }

    /*
     * Step 2
     */
    /* Equation 8 */
    float deltaLPrime = lab2.l - lab1.l;
    /* Equation 9 */
    float deltaCPrime = CPrime2 - CPrime1;
    /* Equation 10 */
    float deltahPrime;
    float CPrimeProduct = CPrime1 * CPrime2;
    if (CPrimeProduct == 0)
        deltahPrime = 0;
    else {
        /* Avoid the fabs() call */
        deltahPrime = hPrime2 - hPrime1;
        if (deltahPrime < -deg180InRad)
            deltahPrime += deg360InRad;
        else if (deltahPrime > deg180InRad)
            deltahPrime -= deg360InRad;
    }
    /* Equation 11 */
    float deltaHPrime = 2.0f * (float) sqrt(CPrimeProduct) *
                        (float) sin(deltahPrime / 2.0f);

    /*
     * Step 3
     */
    /* Equation 12 */
    float barLPrime = (lab1.l + lab2.l) / 2.0f;
    /* Equation 13 */
    float barCPrime = (CPrime1 + CPrime2) / 2.0f;
    /* Equation 14 */
    float barhPrime, hPrimeSum = hPrime1 + hPrime2;
    if (CPrime1 * CPrime2 == 0) {
        barhPrime = hPrimeSum;
    } else {
        if (fabs(hPrime1 - hPrime2) <= deg180InRad)
            barhPrime = hPrimeSum / 2.0f;
        else {
            if (hPrimeSum < deg360InRad)
                barhPrime = (hPrimeSum + deg360InRad) / 2.0f;
            else
                barhPrime = (hPrimeSum - deg360InRad) / 2.0f;
        }
    }
    /* Equation 15 */
    float T = 1.0f - (float) (0.17f * cos(barhPrime - deg2Rad(30.0))) +
              (float) (0.24f * cos(2.0f * barhPrime)) +
              (float) (0.32f * cos((3.0f * barhPrime) + deg2Rad(6.0))) -
              (float) (0.20f * cos((4.0f * barhPrime) - deg2Rad(63.0)));
    /* Equation 16 */
    float deltaTheta = deg2Rad(30.0) *
                       (float) exp(-pow((barhPrime - deg2Rad(275.0)) / deg2Rad(25.0), 2.0));
    /* Equation 17 */
    float R_C = 2.0f * (float) sqrt(pow(barCPrime, 7.0f) /
                                    (pow(barCPrime, 7.0f) + pow25To7));
    /* Equation 18 */
    float S_L = 1 + (float) ((0.015 * pow(barLPrime - 50.0, 2.0)) /
                             sqrt(20 + pow(barLPrime - 50.0, 2.0)));
    /* Equation 19 */
    float S_C = 1 + (0.045f * barCPrime);
    /* Equation 20 */
    float S_H = 1 + (0.015f * barCPrime * T);
    /* Equation 21 */
    float R_T = (float) (-sin(2.0 * deltaTheta)) * R_C;

    /* Equation 22 */
    float deltaE = (float) sqrt(
            pow(deltaLPrime / (k_L * S_L), 2.0) +
            pow(deltaCPrime / (k_C * S_C), 2.0) +
            pow(deltaHPrime / (k_H * S_H), 2.0) +
            (R_T * (deltaCPrime / (k_C * S_C)) * (deltaHPrime / (k_H * S_H))));

    return deltaE;
}

/**
 * compute CIDE2000 distance between point(j, i) and it two neighbor points
 * @param src input image
 * @param out out put result
 */
__global__ void kernel_compute_distance(cv::cuda::PtrStepSz<uchar3> src,int row_index,int col_index,
                                        cv::cuda::PtrStepSz<float> out) {
    int i = threadIdx.x + blockIdx.x * blockDim.x; //行坐标
    int j = threadIdx.y + blockIdx.y * blockDim.y; //列坐标

    if (i >= src.rows || j >= src.cols) {
        return;
    }
    uchar3 bgr1 = src(row_index, col_index);
    uchar3 bgr2 = src(i, j);
    out(i, j) = cide_distance(bgr1,bgr2);
}

void compute_distance(const cv::Mat &src,int row_index,int col_index, cv::Mat &dst) {
    cv::cuda::GpuMat g_src;
    cv::cuda::GpuMat g_dst(src.size(), CV_32FC1);
    g_src.upload(src);

    dim3 block(32, 32);
    dim3 grid((src.rows + block.y - 1) / block.y, (src.cols + block.x - 1) / block.x);
    kernel_compute_distance<<< grid, block, 0 >>>(g_src, row_index,col_index,g_dst);

    g_dst.download(dst);
}

__global__ void kernel_init(){
    printf("kernel initialized\n");
}

void init_cuda() {
    kernel_init<<<1,1,0>>>();
}