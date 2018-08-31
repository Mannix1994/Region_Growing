#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <list>
#include "grow.h"
#include "cuda/cide.h"

using namespace std;
using namespace cv;

grow::grow(double colorThreshold) : colorThreshold(colorThreshold) {
}

/**
 * Function to modify pixel values at a point as per seed pixel
 */
void grow::modifyPixel(Mat &input, int x, int y, Color color) {
    Vec3b &colorC = input.at<Vec3b>(x, y);

    switch (color){
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

void grow::start_grow(Mat input, Mat filled, Mat edgeMap, int row_index, int col_index, Color color) {
    // call cuda to compute all the color distance between every point and seed point.
    // the color distance between input(i,j) and seed point(input(row_index,col_index)
    // is stored in distances(i,j).
    compute_distance(input, row_index, col_index, distances);

    int x, y;
    long int count = 1;
    String s = "";
    Vec3b seed = input.at<Vec3b>(row_index, col_index);
    vector<vector<bool> > reach(input.rows, vector<bool>(input.cols, false));

    list<pair<int, int> > queue;

    reach[row_index][col_index] = true;

    modifyPixel(filled, row_index, col_index, color);

    queue.emplace_back(make_pair(row_index, col_index));

    auto helper_lambda = [&](int lx, int ly) {
        if (colorDistance(lx, ly)) {
            reach[lx][ly] = true;
            queue.emplace_back(make_pair(lx, ly));
            modifyPixel(filled, lx, ly, color);
            ++count;
        } else
            modifyPixel(edgeMap, lx, ly, color);
    };

    while (!queue.empty()) {
        x = queue.front().first;
        y = queue.front().second;
        queue.pop_front();

        //Right Pixel
        if (x + 1 < input.rows && (!reach[x + 1][y]))
            helper_lambda(x + 1, y);

        //Below Pixel
        if (y + 1 < input.cols && (!reach[x][y + 1]))
            helper_lambda(x, y + 1);

        //Left Pixel
        if (x - 1 >= 0 && (!reach[x - 1][y]))
            helper_lambda(x - 1, y);

        //Above Pixel
        if (y - 1 >= 0 && (!reach[x][y - 1]))
            helper_lambda(x, y - 1);

        //Bottom Right Pixel
        if (x + 1 < input.rows && y + 1 < input.cols && (!reach[x + 1][y + 1]))
            helper_lambda(x + 1, y + 1);

        //Upper Right Pixel
        if (x + 1 < input.rows && y - 1 >= 0 && (!reach[x + 1][y - 1]))
            helper_lambda(x + 1, y - 1);

        //Bottom Left Pixel
        if (x - 1 >= 0 && y + 1 < input.cols && (!reach[x - 1][y + 1]))
            helper_lambda(x - 1, y + 1);

        //Upper left Pixel
        if (x - 1 >= 0 && y - 1 >= 0 && (!reach[x - 1][y - 1]))
            helper_lambda(x - 1, y - 1);
    }
}
