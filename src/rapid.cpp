#include "rapid.hpp"
#include <vector>
#include <opencv2/opencv.hpp>
#include <fstream>

void rapid::toTXT()
{
    std::vector<std::vector<cv::Point>> theContours = gettheCoutours(1);// 获得采样后的点
    
    for(size_t i = 0; i < theContours.size(); i++)
    {
        int index = 0;
        outfile << "CONST robtarget myPath" << i << "{" << theContours[i].size() << "} := [\n";
        for(size_t j = 0; j < theContours[i].size(); j++)
        {
            float wx = base_x + scale * theContours[i][j].x;
            float wy = base_y + scale * theContours[i][j].y;
            float wz = base_z;// Z坐标不变

            outfile << "[[" << wx << "," << wy << "," << wz
                << "],[0.00139266,-0.491072,0.870802,-0.0234724],[0,-1,-1,0],[9E+09,9E+09,9E+09,9E+09,9E+09,9E+09]]";
            if(index != theContours[i].size() - 1)
                outfile << ",\n";

            index++;
        }
        outfile << "\n];\n";
    }
}