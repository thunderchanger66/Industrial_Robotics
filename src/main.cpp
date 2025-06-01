#include <iostream>
#include <opencv2/opencv.hpp>

int main(int, char**)
{
    cv::Mat img = cv::imread("E:\\STUDY\\Junior.down\\Industrial_Robotics\\resources\\dog.jpg");

    cv::imshow("dog", img);
    cv::waitKey(0);
    return 0;
}
