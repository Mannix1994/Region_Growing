#include <opencv2/opencv.hpp>
#include "region_grow/grow.h"
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

    grow M(12);
    timer.rlog("part2");

    M.start_grow(denoise, // a blur mat of origin mat
                 filled,  // a filled mat
                 edgeMap, // a edge mat
                 267,     // seed point's row index
                 108,     // seed point's col index
                 grow::YELLOW);
    timer.rlog("part3");

    M.start_grow(denoise, filled, edgeMap, 91, 468, grow::YELLOW);
    timer.rlog("part4");

    M.start_grow(denoise, filled, edgeMap, 50, 173, grow::YELLOW);
    timer.rlog("part5");

    M.start_grow(denoise, filled, edgeMap, 247, 422, grow::RED);
    timer.rlog("part6");

//    imshow("Original Image", denoise);
    imshow("filled Image", filled);
//    imshow("EdgeMap Image", edgeMap);
    waitKey(0);
    destroyAllWindows();

    return 0;
}