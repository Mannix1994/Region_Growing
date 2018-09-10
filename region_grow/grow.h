/*
 * Ciff
 * Copyright (C) 2015 Amir Hassan <amir@viel-zu.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#ifndef GROW_H
#define GROW_H

#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
//using namespace cv;

class grow {
public:
    typedef enum {
        RED, GREEN, YELLOW, BLACK, WHITE
    } Color;

    /**
     * Constructor sets default values to colorThreshold
     * @param colorThreshold if color distance between point(i,j) and seed point
     * is lower than colorThreshold, the point(i,j) and seed point has "similar"
     * color.
     */
    explicit grow(double colorThreshold = 12);

    /**
     * Region Growing algorithm, which is flood type algorithm
     * @param input a BGR Mat
     * @param filled output image with filled buoys
     * @param edgeMap output image with only edges of final blobs
     * @param row_index Seed Pixel row index
     * @param col_index Seed Pixel col index
     * @param color to determine the color to be filled with
     * @param size the region size you want fill. smaller --> faster.
     * if parameter size is not set, the region will be entire Mat.
     */
    void start_grow(const cv::Mat& input, cv::Mat& filled, cv::Mat& edgeMap,
            int row_index, int col_index, Color color,cv::Size size=cv::Size(0,0));

    /**
     * get the sub rect, whose center point is (row_index,col_index), of src_size
     * @param src_size input size of Mat or rect
     * @param row the row index of center point
     * @param col the cols index of center point
     * @param point point the new location of (row_index,col_index) in sub rect
     * @param size sub rect's size
     * @return a sub rect
     */
    static cv::Rect get_sub_rect(const cv::Size &src_size, int row, int col,
            cv::Point &point, cv::Size size=cv::Size(100, 100));

private:
    double colorThreshold;
    cv::Mat distances;

    /**
     * return if the color distance dist between pixel(i0,j0) and seed pixel is smaller than colorThreshold
     * @param i0 pixel's row index
     * @param j0 pixel's col index
     * @return return true if dist<colorThreshold
     */
    inline bool colorDistance(int i0, int j0) {
        return distances.at<float>(i0, j0) < colorThreshold;
    }

    /**
     * modify pixel's color
     * @param input the mat has the pixel you wang change color
     * @param row pixel's row index
     * @param col pixel's col index
     * @param color the Color you want to change pixel to
     */
    void modifyPixel(cv::Mat &input, int row, int col, Color color);
};

#endif