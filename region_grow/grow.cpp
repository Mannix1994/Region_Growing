#include <opencv2/opencv.hpp>
#include <string>
#include <list>
#include "Color.hpp"
#include "grow.h"

using namespace std;
using namespace cv;

/**
 * Constructor sets default values to colorThreshold and whiteThreshold
 */
grow::grow(double colorThreshold) : colorThreshold(colorThreshold)
{

}

/**
 * Function to determine the color difference between two pixel points.
 * This function also calculates the color distance between white pixel
 * and input pixel to deal overexposed regions of buoy.
 * This function implements CIEDE2000 algorithm to find the distance in color
 */
bool grow::colorDistance(const Vec3b& a, const Vec3b& b, int colorThreshold)
{
    std::vector<double> ca, cb, cw(3, 255);

    ca.push_back((double) a[2]);
    ca.push_back((double) a[1]);
    ca.push_back((double) a[0]);

    cb.push_back((double) b[2]);
    cb.push_back((double) b[1]);
    cb.push_back((double) b[0]);

    float dist1 = (float)kallaballa::ciede2000_distance(ca, cb);

    return dist1 <= colorThreshold;
}

/**
 * Function to modify pixel values at a point as per seed pixel
 */
void grow::modifyPixel(Mat &input, int row, int col, Color color) {
    Vec3b &colorC = input.at<Vec3b>(row, col);

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

void grow::start_grow(Mat input, Mat filled, Mat edgeMap, int sX, int sY, Color color)
{
    int x, y;
    long int count = 1;
    String s = "";
    Vec3b seed = input.at<Vec3b>(sX, sY);
    vector < vector<bool> > reach(input.rows, vector<bool>(input.cols, false));

    list < pair<int, int> > queue;

    reach[sX][sY] = true;

    modifyPixel(filled, sX, sY, color);

    queue.emplace_back(make_pair(sX, sY));

    auto helper_lambda = [&](int lx, int ly)
        {
            if (colorDistance(seed, input.at<Vec3b>(lx , ly), colorThreshold))
            {
                reach[lx][ly] = true;
                queue.emplace_back(make_pair(lx, ly));
                modifyPixel(filled, lx, ly, color);
                ++count;
            }
            else
                modifyPixel(edgeMap, lx, ly, color);
        };

    while (!queue.empty())
    {
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