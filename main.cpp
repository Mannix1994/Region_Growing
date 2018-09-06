#include <cstdlib>
#include <opencv2/opencv.hpp>
#include "region_grow/grow.h"
#include "region_grow/timer.h"

using namespace std;
using namespace cv;

int main(int argc, char const **argv) {
    Timer timer;
    Mat src, denoise;

    src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    fastNlMeansDenoisingColored(src, denoise, 2);
    medianBlur(denoise, denoise, 3);
    cuda::setDevice(0);
    timer.rlog("part0");
    cuda::GpuMat g_denoise = grow::BGR2Lab(denoise);
    timer.rlog("part1");

    Mat edgeMap(denoise.rows, denoise.cols, CV_8UC3, Scalar(0, 0, 0));
    Mat filled(denoise.rows, denoise.cols, CV_8UC3, Scalar(0, 0, 0));

    grow M(12.5);
    timer.rlog("part2");

    M.start_grow(g_denoise, filled, edgeMap, 267, 108, grow::YELLOW, Size(200,200));
    timer.rlog("part3");

    M.start_grow(g_denoise, filled, edgeMap, 91, 468, grow::GREEN);
    timer.rlog("part4");

    M.start_grow(g_denoise, filled, edgeMap, 50, 173, grow::RED);
    timer.rlog("part5");

    M.start_grow(g_denoise, filled, edgeMap, 247, 422, grow::WHITE);
    timer.rlog("part6");

//    imshow("Original Image", denoise);
    imshow("filled Image", filled);
//    imshow("EdgeMap Image", edgeMap);
    waitKey(0);
    destroyAllWindows();

    return 0;
}