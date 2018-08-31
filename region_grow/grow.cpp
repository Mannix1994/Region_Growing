#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <string>
#include <list>
#include "grow.h"
#include "cuda/cide.h"

using namespace std;
using namespace cv;

/**
 * Constructor sets default values to colorThreshold and whiteThreshold
 */
grow::grow(double colorThreshold) : colorThreshold(colorThreshold) {
    init_cuda();
}

/**
 * Function to modify pixel values at a point as per seed pixel
 */
void grow::modifyPixel(Mat &input, int x, int y, int colorflag) {
    Vec3b &colorC = input.at<Vec3b>(x, y);

    //Yellow Pixel
    if (colorflag == 2) {
        colorC[0] = 0;
        colorC[1] = 255;
        colorC[2] = 255;
    }

    //Green Pixel
    if (colorflag == 3) {
        colorC[0] = 0;
        colorC[1] = 255;
        colorC[2] = 0;
    }

    //Red Pixel
    if (colorflag == 4) {
        colorC[0] = 0;
        colorC[1] = 0;
        colorC[2] = 255;
    }
}

/**
 * Region Growing algorithm, which is flood type algorithm
 * filled -> output image with filled buoys
 * edgeMap -> output image with only edges of final blobs
 * sX --> Seed Pixel x value
 * sY --> Seed Pixel y value
 * colorflag --> to determine the color to be filled with
 */
void grow::start_grow(Mat input, Mat filled, Mat edgeMap, int row_index, int col_index, int colorflag) {
    compute_distance(input, row_index, col_index, distances);

    int x, y;
    long int count = 1;
    String s = "";
    Vec3b seed = input.at<Vec3b>(row_index, col_index);
    vector<vector<bool> > reach(input.rows, vector<bool>(input.cols, false));

    list<pair<int, int> > queue;

    reach[row_index][col_index] = true;

    modifyPixel(filled, row_index, col_index, colorflag);

    queue.emplace_back(make_pair(row_index, col_index));

    auto helper_lambda = [&](int lx, int ly) {
        if (colorDistance(lx, ly)) {
            reach[lx][ly] = true;
            queue.emplace_back(make_pair(lx, ly));
            modifyPixel(filled, lx, ly, colorflag);
            ++count;
        } else
            modifyPixel(edgeMap, lx, ly, colorflag);
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
