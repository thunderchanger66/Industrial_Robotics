#include "process.hpp"
#include <iostream>

process::process()
{
    // Show the raw picture
    img = cv::imread("E:\\STUDY\\Junior.down\\Industrial_Robotics\\resources\\dog.jpg");
    if(img.empty())
    {
        std::cerr << "Load image faild!" << std::endl;
        return;
    }
    cv::imshow("Raw", img);
    //cv::waitKey(0);
}

process::~process()
{
    cv::destroyAllWindows();
}

void process::Process()
{
    // Turn into gray
    cv::Mat gray;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);

    // 二值化
    cv::Mat binary;
    cv::threshold(gray, binary, 100, 255, cv::THRESH_BINARY_INV); //反向，白线黑底更容易提取轮廓
    cv::imshow("Binary", binary);
    cv::moveWindow("Binary", 100, 100); // 把窗口移动到屏幕内(100,100)的位置
    //cv::waitKey(0);

    // Find contours
    std::vector<std::vector<cv::Point>> contours;
    /*----------
    Vec4i 是长度为4的向量，表示每个轮廓的：
    hierarchy[i][0]：下一个轮廓的索引
    hierarchy[i][1]：上一个轮廓的索引
    hierarchy[i][2]：第一个子轮廓的索引
    hierarchy[i][3]：父轮廓的索引
    ----------*/
    std::vector<cv::Vec4i> hierarchy;// 保存轮廓层级关系
    cv::findContours(binary, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    // Show contours
    cv::Mat cont = img.clone();
    for(size_t i = 0; i < contours.size(); i++)
        cv::drawContours(cont, contours, i, cv::Scalar(0, 0, 255), 2);
    cv::imshow("Contours", cont);
    cv::waitKey(0);
}
