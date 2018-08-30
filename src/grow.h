#ifndef GROW_H
#define GROW_H

#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

class grow {
public:
    explicit grow(double colorThreshold = 12);

    void start_grow(Mat input, Mat filled, Mat edgeMap, int row_index, int col_index, int colorflag);

private:
    double colorThreshold;
    Mat distances;

    inline bool colorDistance(int i0, int j0){
        return distances.at<float>(i0, j0) < colorThreshold;
    }

    void modifyPixel(Mat &input, int x, int y, int colorflag);
};

#endif