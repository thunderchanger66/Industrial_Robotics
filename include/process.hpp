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
    float angle_thresh;// 角度阈值，代替曲率，当角度小于阈值时，则为曲率较大的点，需要保留

    void gettheCoutours1();// 等距
    void gettheCoutours2();// 曲率特征

    int pointNum; //总点数
    cv::Mat binary; // 为了做亚像素定位放在了这里，需要灰度图

public:
    process(float min_dist = 10.0f, float curve = CV_PI / 3);// 调试角度，先测试45度；90度；180度; 现在采用60度
    ~process();
    void getContours();
    std::vector<std::vector<cv::Point>> gettheCoutours(int type = 1);// type == 1为等距，type == 0 为曲率特征
    void hierachytest();// 测试轮廓顺序
    void set_min_dist(float num) { min_dist = num; }
    void set_angle_thresh(float num) { angle_thresh = num; }
    int getpointNum() const { return pointNum; }
    //float calAngle(cv::Point2f a, cv::Point2f b, cv::Point2f c);// NO need
};