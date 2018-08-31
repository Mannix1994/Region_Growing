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