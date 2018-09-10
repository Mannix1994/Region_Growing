#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <list>
#include "grow.h"
#include "cuda/cide.h"
#include <stdexcept>
#include "timer.h"

using namespace std;
using namespace cv;

grow::grow(double colorThreshold) : colorThreshold(colorThreshold) {
//    init_cuda();
}

/**
 * Function to modify pixel values at a point as per seed pixel
 */
void grow::modifyPixel(Mat &input, int row_index, int col_index, Color color) {
    Vec3b &colorC = input.at<Vec3b>(row_index, col_index);

    switch (color) {
        case RED:
            colorC[0] = 0;
            colorC[1] = 0;
            colorC[2] = 255;
            break;
        case GREEN:
            colorC[0] = 0;
            colorC[1] = 255;
            colorC[2] = 0;
            break;
        case YELLOW:
            colorC[0] = 0;
            colorC[1] = 255;
            colorC[2] = 255;
            break;
        case BLACK:
            colorC[0] = 0;
            colorC[1] = 0;
            colorC[2] = 0;
            break;
        case WHITE:
            colorC[0] = 255;
            colorC[1] = 255;
            colorC[2] = 255;
            break;
        default:
            break;
    }
}

void grow::start_grow(const cuda::GpuMat &input, Mat &filled, Mat &edgeMap, int row_index, int col_index, Color color, Size size) {
    if(input.size() != filled.size() || input.size() != edgeMap.size()){
        throw std::runtime_error("The sizes of input, filled and edgeMap are not equal with each other");
    }
    if(size == Size(0,0))
        size = input.size();

    Point p;
    Rect ROI = get_sub_rect(filled.size(), row_index, col_index, p, size);

    cuda::GpuMat input_sub = input(ROI);
    Mat filled_sub = filled(ROI);
    Mat edgeMap_sub = edgeMap(ROI);

    row_index = p.y;
    col_index = p.x;
    // call cuda to compute all the color distance between every point and seed point.
    // the color distance between input(i,j) and seed point(input(row_index,col_index)
    // is stored in distances(i,j).
    Timer timer;
    compute_distance(input_sub, row_index, col_index, distances);
    timer.rlog("compute_distance");

    int x, y;
    long int count = 1;
    String s = "";
    vector<vector<bool> > reach(input_sub.rows, vector<bool>(input_sub.cols, false));

    list<pair<int, int> > queue;

    reach[row_index][col_index] = true;

    modifyPixel(filled_sub, row_index, col_index, color);

    queue.emplace_back(make_pair(row_index, col_index));

    auto helper_lambda = [&](int lx, int ly) {
        if (colorDistance(lx, ly)) {
            reach[lx][ly] = true;
            queue.emplace_back(make_pair(lx, ly));
            modifyPixel(filled_sub, lx, ly, color);
            ++count;
        } else
            modifyPixel(edgeMap_sub, lx, ly, color);
    };

    while (!queue.empty()) {
        x = queue.front().first;
        y = queue.front().second;
        queue.pop_front();

        //Right Pixel
        if (x + 1 < input_sub.rows && (!reach[x + 1][y]))
            helper_lambda(x + 1, y);

        //Below Pixel
        if (y + 1 < input_sub.cols && (!reach[x][y + 1]))
            helper_lambda(x, y + 1);

        //Left Pixel
        if (x - 1 >= 0 && (!reach[x - 1][y]))
            helper_lambda(x - 1, y);

        //Above Pixel
        if (y - 1 >= 0 && (!reach[x][y - 1]))
            helper_lambda(x, y - 1);

        //Bottom Right Pixel
        if (x + 1 < input_sub.rows && y + 1 < input_sub.cols && (!reach[x + 1][y + 1]))
            helper_lambda(x + 1, y + 1);

        //Upper Right Pixel
        if (x + 1 < input_sub.rows && y - 1 >= 0 && (!reach[x + 1][y - 1]))
            helper_lambda(x + 1, y - 1);

        //Bottom Left Pixel
        if (x - 1 >= 0 && y + 1 < input_sub.cols && (!reach[x - 1][y + 1]))
            helper_lambda(x - 1, y + 1);

        //Upper left Pixel
        if (x - 1 >= 0 && y - 1 >= 0 && (!reach[x - 1][y - 1]))
            helper_lambda(x - 1, y - 1);
    }
}

cv::Rect grow::get_sub_rect(const cv::Size &src_size, int row, int col, Point &point, Size size) {
    if (row < 0 || row >= src_size.height || col < 0 || col >= src_size.width) {
        throw runtime_error("row index or cols index is out of input");
    }
    if (size.width % 2 != 0 || size.height % 2 != 0) {
        throw runtime_error("size.width and size.height should be even numbers");
    }

    // compute rect's x and y
    Rect rect;
    rect.x = (col - size.width / 2) > 0 ? (col - size.width / 2) : 0;
    rect.y = (row - size.height / 2) > 0 ? (row - size.height / 2) : 0;

    // how mush left overflow
    int left_o = (col - size.width / 2)>0?0:(col - size.width / 2);
    // how mush right overflow
    int right_o = (col + size.width/2)<src_size.width?0:(src_size.width-1-(col + size.width/2));
    // how much above overflow
    int above_o = (row - size.height/2)>0?0:((row - size.height/2));
    // how mush below overflow
    int below_o = (row + size.height/2)<src_size.height?0:(src_size.height-1-(row + size.height/2));

    // compute rect width and height
    rect.width = size.width + left_o + right_o;
    rect.height = size.height + above_o + below_o;

    // compute new location of point(row,col) in "rect"
    point.x = col - rect.x;
    point.y = row - rect.y;

    return rect;
}

cv::cuda::GpuMat grow::BGR2Lab(const cv::Mat &bgr) {
    CV_DbgAssert(!bgr.empty() && "bgr is empty");
    static cuda::GpuMat lab;
    bgr2lLab(bgr,lab);
    return lab;
}
