## Region Grow
Region Grow是一种，区域生长算法，是泛洪法的一个变种。
* [master]()分支:这是从[]()复制过来的，重新调整了项目的架构和添加示例。
* [single]()分支：使用CUDA加速的区域生长算法。计算颜色距离使用的是单精度。
* [double]()分支：使用CUDA加速的区域生长算法。计算颜色距离使用的是双精度，
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
* OpenCV >= 3.2.0
