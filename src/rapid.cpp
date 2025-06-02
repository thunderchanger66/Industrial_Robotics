#include "rapid.hpp"
#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>

void rapid::toTXT()
{
    std::vector<std::vector<cv::Point>> theContours = gettheCoutours(1);// 获得采样后的点

    int index = 1;
    outfile << "CONST robtarget myPath{" << getpointNum() << "} := [\n";
    for(size_t i = 0; i < theContours.size(); i++)
    {
        for(size_t j = 0; j < theContours[i].size(); j++)
        {
            float wx = base_x + scale * theContours[i][j].x;
            float wy = base_y + scale * theContours[i][j].y;
            float wz = base_z;// Z坐标不变

            // outfile << "CONST robtarget p" << index << " := [["
            //     << wx << "," << wy << "," << wz << "],[1,0,0,0],[0,0,0,0],[9E9,9E9,9E9,9E9,9E9,9E9]];\n";
            outfile << "[[" << wx << "," << wy << "," << wz
                << "],[0.00139266,-0.491072,0.870802,-0.0234724],[0,-1,-1,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]]";
            if(index != getpointNum())
                outfile << ",\n";

            //rapidcode << "MoveL p" << index << ", v100, z5, AW_Gun;\n";// 修改后使用循环调用MoveL，不使用这个

            index++;
        }
    }
    outfile << "\n];\n";
}