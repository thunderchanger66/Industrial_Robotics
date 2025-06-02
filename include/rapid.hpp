#pragma once
#include <opencv2/opencv.hpp>
#include "process.hpp"
#include <fstream>

class rapid: public process
{
private:
    float scale;// 比例因子，默认1mm=1pixel
    float base_x;// 机械臂的起始点
    float base_y;
    float base_z;
    std::ofstream outfile;// 文件操作的对象，要输出到path.txt；这个是target点

public:
    rapid(float scale = 0.5f, float base_x = 1263.7f, float base_y = 0.0f, float base_z = 938.01f): 
        scale(scale), base_x(base_x), base_y(base_y), base_z(base_z), 
        outfile("E:\\STUDY\\Junior.down\\Industrial_Robotics\\output\\path.txt") {}
    ~rapid() { outfile.close(); }
    void toTXT();
    void set_scale(float num) { scale = num; }
    void set_base(float num1, float num2, float num3) { base_x = num1; base_y = num2; base_x = num3; }
};