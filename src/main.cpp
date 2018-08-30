#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>
#include <sstream>
#include <list>
#include "Color.hpp"
#include <chrono>
#include "grow.h"
#include "timer.h"

using namespace std;
using namespace cv;
using namespace std::chrono;

int main(int argc, char const **argv) {
    Timer timer;
    Mat src, denoise;

    src = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    fastNlMeansDenoisingColored(src, denoise, 2);
    medianBlur(denoise, denoise, 3);
    timer.rlog("part1");

    Mat edgeMap(denoise.rows, denoise.cols, CV_8UC3, Scalar(0, 0, 0));
    Mat filled(denoise.rows, denoise.cols, CV_8UC3, Scalar(0, 0, 0));

    grow M;
    M.setThresholds(12, 15);
    timer.rlog("part2");

    M.start_grow(denoise, filled, edgeMap, 267, 108, 2);
    timer.rlog("part3");

    M.start_grow(denoise, filled, edgeMap, 91, 468, 2);
    timer.rlog("part4");

    M.start_grow(denoise, filled, edgeMap, 50, 173, 4);
    timer.rlog("part4");

//    imshow("Original Image", denoise);
    imshow("filled Image", filled);
//    imshow("EdgeMap Image", edgeMap);
    waitKey(0);
    destroyAllWindows();

    return 0;
}