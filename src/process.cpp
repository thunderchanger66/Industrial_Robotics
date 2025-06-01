#include "process.hpp"

process::process()
{
    img = cv::imread("E:\\STUDY\\Junior.down\\Industrial_Robotics\\resources\\dog.jpg");
    cv::imshow("Raw", img);
    cv::waitKey(0);
}

process::~process()
{
    cv::destroyAllWindows();
}