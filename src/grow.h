#ifndef GROW_H
#define GROW_H

#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace std;
using namespace cv;

class grow {
public:
    explicit grow(int colorThreshold = 20, int whiteThreshold = 10);

    void start_grow(Mat input, Mat filled, Mat edgeMap, const int row_index, const int col_index, const int colorflag);

    void setThresholds(const int colorThreshold, const int whiteThreshold);

private:
    int colorThreshold;
    int whiteThreshold;

    Mat distances;
    bool colorDistance(int i0, int j0);
    void modifyPixel(Mat &input, const int x, const int y, const int colorflag);
};

#endif