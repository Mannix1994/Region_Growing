//
// Created by creator on 18-8-30.
//

#ifndef RG_CIDE_H
#define RG_CIDE_H

#include <opencv2/opencv.hpp>
void init_cuda();
void compute_distance(const cv::cuda::GpuMat &src,int row_index,int col_index,cv::Mat &dst);
void bgr2lLab(const cv::Mat &bgr, cv::cuda::GpuMat &lab);

#endif //RG_CIDE_H
