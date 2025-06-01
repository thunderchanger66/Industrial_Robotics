#pragma once
#include <opencv2/opencv.hpp>

class process
{
private:
    cv::Mat img;// Raw picture

public:
    process();
    void Process();
    ~process();
};