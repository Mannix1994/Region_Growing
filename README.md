# 中文文档
## Region Grow
Region Grow是一种区域生长算法，是泛洪法的一个变种。
* [master](https://github.com/Mannix1994/Region_Growing/tree/master)
分支:这是从Nightfury18的项目[Region_Growing](https://github.com/Nightfury18/Region_Growing)复制过来的，重新调整了项目的架构和添加示例。
* [single](https://github.com/Mannix1994/Region_Growing/tree/single)
分支：使用CUDA加速的区域生长算法。计算颜色距离使用的是单精度。
* [double](https://github.com/Mannix1994/Region_Growing/tree/double)
分支：使用CUDA加速的区域生长算法。计算颜色距离使用的是双精度，
速度比单精度稍微慢一些。
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

# English Document
## Region Grow
Region Grow is a kind of floodfill-like algorithm. It is used to region growing.
* Branch [master](https://github.com/Mannix1994/Region_Growing/tree/master)
: Copied from Nightfury18' project [Region_Growing](https://github.com/Nightfury18/Region_Growing),
and I reconstructed project and add some example.
* Branch [single](https://github.com/Mannix1994/Region_Growing/tree/single)
： CUDA accelerated Region Grow. Compute color distance with single precision(float).
* Branch [double](https://github.com/Mannix1994/Region_Growing/tree/double)
：CUDA accelerated Region Grow. Compute color distance with double precision(double).
It is slower than single precision.
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
