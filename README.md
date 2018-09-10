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
* OpenCV >= 3.2.0


# English Document
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
* OpenCV >= 3.2.0
