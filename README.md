# 中文文档
## Region Grow
Region Grow是一种区域生长算法，是泛洪法的一个变种。算法的基本思想是，计算周围像素
与种子点的颜色距离，如果小于某一个阈值，就认为是同一个区域；然后在计算是同一个区域
的像素周围的像素与种子点的颜色距离，如此往复。计算颜色距离分为两步，先把BGR转为Lab
，然后使用CIEDE2000的公式来计算颜色距离。
* [master](https://github.com/Mannix1994/Region_Growing/tree/master)
分支:这是从Nightfury18的项目[Region_Growing](https://github.com/Nightfury18/Region_Growing)复制过来的，重新调整了项目的架构和添加示例。
* [single](https://github.com/Mannix1994/Region_Growing/tree/single)
分支：使用CUDA加速的区域生长算法。和原算法相比，计算颜色距离是使用GPU来计算的。
* [performance](https://github.com/Mannix1994/Region_Growing/tree/performance)
分支：使用CUDA加速的区域生长算法。适用于特别需要高性能的场景。和single分支相比，
本分支是先把一个BGR-Mat转换成Lab-Mat(可在自线程中完成，从而实现异步)，然后再计算颜色距离。

## 用法
1. 下载本项目
2. 把region_grow复制到你的项目的根目录里
3. 修改你的项目的CMakeLists.txt
```cmake
# 添加子目录
add_subdirectory(region_grow)

# 添加库
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} 
                        region_grow  #添加region_grow
)
```
可参考本项目的CMakeLists.txt。  
4. 代码示例
```
#include ...
#include "region_grow/grow.h"

void fun(){
    grow M(12,15);
    
    Mat input = imread(..);
    Mat dst;
    blur(input,dst,...);
    Mat filled;
    Mat edge;
    
    M.start_grow(dst,filled,edge,9,48,grow::RED);
}
```
完整的示例在main.cpp中。

## 依赖
* CUDA >= 8.0。
* OpenCV >= 3.2.0。编译OpenCV时需要添加CUDA支持。

编译OpenCV步骤
* 安装CUDA
* 安装OpenCV依赖
```
# ubuntu 16.04
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev libgtk-3-dev libgtkglext1-dev libgstreamer1.0-dev libavresample-dev  libgphoto2-dev libopenblas-dev libatlas-dev doxygen libgstreamer-ocaml-dev libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base0.10-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamer0.10-dev libgstreamer1.0-dev libgstreamermm-1.0-dev tesseract-ocr-dev liblept5 libleptonica-dev liblapacke-dev python3-numpy libv4l-dev python3-numpy python3-pip python-pip libprotobuf-dev libhdf5-dev libgflags-dev

# ubuntu 18.04
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev libgtk-3-dev libgtkglext1-dev libgstreamer1.0-dev libavresample-dev  libgphoto2-dev libopenblas-dev libatlas-base-dev doxygen libgstreamer-ocaml-dev libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamer1.0-dev libgstreamermm-1.0-dev libtesseract-dev liblept5 libleptonica-dev liblapacke-dev libgstreamer-opencv1.0-0 libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamerd-3-dev libgstreamermm-1.0-dev libv4l-dev libprotobuf-dev libhdf5-dev libgflags-dev python3-numpy
```
* 进入到OpenCV源文件的根目录中
```
mkdir release

cd release

cmake -D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=/usr/local/opencv320 \
	-D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib-3.2.0/modules \
	-D WITH_LIBV4L=ON \
	-D WITH_CUDA=ON \
	-D CUDA_ARCH_BIN="3.0 5.0 5.2" \
	-D CUDA_ARCH_PTX="3.0 5.0 5.2" \
	-D ENABLE_FAST_MATH=ON \
	-D CUDA_FAST_MATH=ON \
	-D WITH_CUBLAS=ON \
	-D WITH_NVCUVID=ON \
	-D CUDA_GENERATION=Auto \
	-D WITH_TBB=ON \
	-D WITH_OPENMP=ON \
	-D WITH_OPENGL=ON ..
	
# 没有错误时
make -j 12 # 12要替换成你的CPU的核心数量

sudo make install
```

## Region Grow
Region Grow is a kind of floodfill-like algorithm. You can look the code to find out 
how it works.
* Branch [master](https://github.com/Mannix1994/Region_Growing/tree/master)
: Copied from Nightfury18's project [Region_Growing](https://github.com/Nightfury18/Region_Growing),
and I reconstructed project and add some example.
* Branch [single](https://github.com/Mannix1994/Region_Growing/tree/single)
： CUDA accelerated Region Grow. Compute color distance with single precision(float).

* Branch [performance](https://github.com/Mannix1994/Region_Growing/tree/performance)
：CUDA accelerated Region Grow. This branch is a high performance version. Firstly,
Turn a BGR Mat to a Lab Mat(which can be finished in a sub thread, main thread 
can do other task), secondly call compute color distance.

## How to Use
1. download this project
2. copy derectory region_grow to root directory of your project
3. modify CMakeLists.txt
```cmake
# add subdirectory
add_subdirectory(region_grow)

# add library
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} 
                        region_grow  #add region_grow
)
```
CMakeLists.txt has complete example.  
4. code example
```
#include ...
#include "region_grow/grow.h"

void fun(){
    grow M(12,15);
    
    Mat input = imread(..);
    Mat dst;
    blur(input,dst,...);
    Mat filled;
    Mat edge;
    
    M.start_grow(dst,filled,edge,9,48,grow::RED);
}
```
main.cpp has complete example.

## Dependency
* CUDA >= 8.0
* OpenCV >= 3.2.0. You should compile OpenCV with CUDA.

Compile Steps
* Install CUDA
* Install dependencies of OpenCV
```
# ubuntu 16.04
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev libgtk-3-dev libgtkglext1-dev libgstreamer1.0-dev libavresample-dev  libgphoto2-dev libopenblas-dev libatlas-dev doxygen libgstreamer-ocaml-dev libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base0.10-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamer0.10-dev libgstreamer1.0-dev libgstreamermm-1.0-dev tesseract-ocr-dev liblept5 libleptonica-dev liblapacke-dev python3-numpy libv4l-dev python3-numpy python3-pip python-pip libprotobuf-dev libhdf5-dev libgflags-dev

# ubuntu 18.04
sudo apt-get install build-essential cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev libgtk-3-dev libgtkglext1-dev libgstreamer1.0-dev libavresample-dev  libgphoto2-dev libopenblas-dev libatlas-base-dev doxygen libgstreamer-ocaml-dev libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamer1.0-dev libgstreamermm-1.0-dev libtesseract-dev liblept5 libleptonica-dev liblapacke-dev libgstreamer-opencv1.0-0 libgstreamer-plugins-bad1.0-dev libgstreamer-plugins-base1.0-dev libgstreamer-plugins-good1.0-dev libgstreamerd-3-dev libgstreamermm-1.0-dev libv4l-dev libprotobuf-dev libhdf5-dev libgflags-dev python3-numpy
```
* unzip opencv_contrib to root directory of OpenCV source code.
* Enter root directory of OpenCV source code.
```
mkdir release

cd release

cmake -D CMAKE_BUILD_TYPE=RELEASE \
	-D CMAKE_INSTALL_PREFIX=/usr/local/opencv320 \
	-D OPENCV_EXTRA_MODULES_PATH=../opencv_contrib-3.2.0/modules \
	-D WITH_LIBV4L=ON \
	-D WITH_CUDA=ON \
	-D CUDA_ARCH_BIN="3.0 5.0 5.2" \
	-D CUDA_ARCH_PTX="3.0 5.0 5.2" \
	-D ENABLE_FAST_MATH=ON \
	-D CUDA_FAST_MATH=ON \
	-D WITH_CUBLAS=ON \
	-D WITH_NVCUVID=ON \
	-D CUDA_GENERATION=Auto \
	-D WITH_TBB=ON \
	-D WITH_OPENMP=ON \
	-D WITH_OPENGL=ON ..
	
# when there is no error
make -j 12 # replace 12 with the number of your CPU cores

sudo make install
```
