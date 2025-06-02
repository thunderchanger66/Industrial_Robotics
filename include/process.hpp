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

    float min_dist;// 等距采样间隔，单位像素
    float curve;// 曲率阈值，最小值，越调大取得点越少

    void gettheCoutours1();// 等距
    void gettheCoutours2();// 曲率特征

    int pointNum; //总点数

public:
    process(float min_dist = 10.0f, float curve = 0.789f);
    ~process();
    void getContours();
    std::vector<std::vector<cv::Point>> gettheCoutours(int type = 1);// type == 1为等距，type == 0 为曲率特征
    void hierachytest();// 测试轮廓顺序
    void set_min_dist(float num) { min_dist = num; }
    void set_curve(float num) { curve = num; }
    int getpointNum() const { return pointNum; }
};