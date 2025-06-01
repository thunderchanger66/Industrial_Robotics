#pragma once
#include <opencv2/opencv.hpp>

class process
{
private:
    cv::Mat img;
    
public:
    process();
    void Process();
    ~process();
};