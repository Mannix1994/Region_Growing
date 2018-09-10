#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <list>
#include "grow.h"
#include "cuda/cide.h"
#include <stdexcept>
#include "../timer.h"

using namespace std;
using namespace cv;

grow::grow(double colorThreshold) : colorThreshold(colorThreshold) {
//    init_cuda();
}

/**
 * Function to modify pixel values at a point as per seed pixel
 */
void grow::modifyPixel(Mat &input, int row, int col, Color color) {
    switch (color){
        case RED:
            input.at<Vec3b>(row, col) = Vec3b(0,0,255);
            break;
        case GREEN:
            input.at<Vec3b>(row, col) = Vec3b(0,255,0);
            break;
        case YELLOW:
            input.at<Vec3b>(row, col) = Vec3b(0,255,255);
            break;
        case BLACK:
            input.at<Vec3b>(row, col) = Vec3b(0,0,0);
            break;
        case WHITE:
            input.at<Vec3b>(row, col) = Vec3b(255,255,255);
            break;
        default:
            break;
    }
}

void grow::start_grow(const Mat &input, Mat &filled, Mat &edgeMap, int row_index, int col_index, Color color, Size size) {

    if(input.size() != filled.size() || input.size() != edgeMap.size()){
        throw std::runtime_error("The sizes of input, filled and edgeMap are not equal with each other");
    }

    Point p;
    Rect roi = get_sub_rect(input.size(), row_index, col_index, p, size);

    Mat input_sub = input(roi);
    Mat filled_sub = filled(roi);
    Mat edgeMap_sub = edgeMap(roi);

    row_index = p.y;
    col_index = p.x;
    // call cuda to compute all the color distance between every point and seed point.
    // the color distance between input(i,j) and seed point(input(row_index,col_index)
    // is stored in distances(i,j).
    Timer timer;
    compute_distance(input_sub, row_index, col_index, distances);
    timer.rlog("compute_distance");

    int row, col;
    long int count = 1;
    vector<vector<bool> > reach(input_sub.rows, vector<bool>(input_sub.cols, false));

    list<pair<int, int> > queue;

    reach[row_index][col_index] = true;

    modifyPixel(filled_sub, row_index, col_index, color);

    queue.emplace_back(make_pair(row_index, col_index));

    auto helper_lambda = [&](int _row, int _col) {
        if (colorDistance(_row, _col)) {
            reach[_row][_col] = true;
            queue.emplace_back(make_pair(_row, _col));
            modifyPixel(filled_sub, _row, _col, color);
            ++count;
        } else
            modifyPixel(edgeMap_sub, _row, _col, color);
    };

    while (!queue.empty()) {
        row = queue.front().first;
        col = queue.front().second;
        queue.pop_front();

        //Right Pixel
        if (row + 1 < input_sub.rows && (!reach[row + 1][col]))
            helper_lambda(row + 1, col);

        //Below Pixel
        if (col + 1 < input_sub.cols && (!reach[row][col + 1]))
            helper_lambda(row, col + 1);

        //Left Pixel
        if (row - 1 >= 0 && (!reach[row - 1][col]))
            helper_lambda(row - 1, col);

        //Above Pixel
        if (col - 1 >= 0 && (!reach[row][col - 1]))
            helper_lambda(row, col - 1);

        //Bottom Right Pixel
        if (row + 1 < input_sub.rows && col + 1 < input_sub.cols && (!reach[row + 1][col + 1]))
            helper_lambda(row + 1, col + 1);

        //Upper Right Pixel
        if (row + 1 < input_sub.rows && col - 1 >= 0 && (!reach[row + 1][col - 1]))
            helper_lambda(row + 1, col - 1);

        //Bottom Left Pixel
        if (row - 1 >= 0 && col + 1 < input_sub.cols && (!reach[row - 1][col + 1]))
            helper_lambda(row - 1, col + 1);

        //Upper left Pixel
        if (row - 1 >= 0 && col - 1 >= 0 && (!reach[row - 1][col - 1]))
            helper_lambda(row - 1, col - 1);
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

