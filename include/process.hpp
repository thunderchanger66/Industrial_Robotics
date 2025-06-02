#pragma once
#include <opencv2/opencv.hpp>
#include <vector>

class process
{
private:
    cv::Mat img;// Raw picture
    std::vector<std::vector<cv::Point>> Contours = {};// 原始轮廓数据
    std::vector<std::vector<cv::Point>> theContours = {};// 最终轮廓数据
    std::vector<cv::Vec4i> hierarchy = {};// 保存轮廓层级关系

    const float min_dist = 10.0f;// 等距采样间隔，单位像素

    void gettheCoutours1();// 等距
    void gettheCoutours2();// 曲率特征

public:
    process();
    ~process();
    void getContours();
    std::vector<std::vector<cv::Point>> gettheCoutours(int type = 1);// type == 1为等距，type == 0 为曲率特征
    void hierachytest();// 测试轮廓顺序
};