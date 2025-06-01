#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class process
{
private:
    cv::Mat img;// Raw picture
    std::vector<std::vector<cv::Point>> Contours;// 原始轮廓数据
    std::vector<std::vector<cv::Point>> theCoutours = {};

    void gettheCoutours1();// 等距
    void gettheCoutours2();// 曲率特征

public:
    process();
    ~process();
    void getContours();
    std::vector<std::vector<cv::Point>> gettheCoutours(int type = 1);// type == 1为等距，type == 0 为曲率特征
};