#include "process.hpp"
#include <iostream>
#include <vector>

process::process(float min_dist, float curve): min_dist(min_dist), curve(curve)
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

void process::getContours()
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
    //std::vector<std::vector<cv::Point>> contours;
    /*----------
    Vec4i 是长度为4的向量，表示每个轮廓的：
    hierarchy[i][0]：下一个轮廓的索引
    hierarchy[i][1]：上一个轮廓的索引
    hierarchy[i][2]：第一个子轮廓的索引
    hierarchy[i][3]：父轮廓的索引
    ----------*/
    cv::findContours(binary, Contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);// 查找所有（包括内层）轮廓
    // Show contours
    cv::Mat cont = img.clone();
    for(size_t i = 0; i < Contours.size(); i++)
        cv::drawContours(cont, Contours, i, cv::Scalar(0, 0, 255), 2);
    cv::imshow("Contours", cont);
    //cv::waitKey(0);
}

void process::gettheCoutours1()// 等距，使用欧氏距离
{
    theContours.clear();// 注意先清零
    theContours.resize(Contours.size());// 若要使用索引访问则先要分配空间
    for(size_t i = 0; i < Contours.size(); i++)
    {
        float sum = 0.0f;
        theContours[i].push_back(Contours[i][0]);// 存入每个轮廓第一个点
        for(size_t j = 1; j < Contours[i].size(); j++)// 注意这里从1开始
        {
            float dx = Contours[i][j].x - Contours[i][j - 1].x;
            float dy = Contours[i][j].y - Contours[i][j - 1].y;
            float dist = std::sqrt(dx * dx + dy * dy);
            sum+=dist;
            if(sum >= min_dist)
            {
                theContours[i].push_back(Contours[i][j]);
                sum = 0.0f;
            }
        }
    }
    cv::Mat cont = img.clone();// 显示等距后的结果
    for(size_t i = 0; i < theContours.size(); i++)
        cv::drawContours(cont, theContours, i, cv::Scalar(255, 0, 0), 2);
    cv::imshow("theContours1", cont);
    cv::waitKey(0);
}

void process::gettheCoutours2()// 曲率特征
{
    theContours.clear();// 注意先清零
    theContours.resize(Contours.size());// 若要使用索引访问则先要分配空间

    // 匿名函数，获得夹角
    auto calAngle = [](cv::Point2f a, cv::Point2f b, cv::Point2f c)
    {
        cv::Point2f ab = b - a;
        cv::Point2f cb = b - c;// 相对的向量
        float dot = ab.dot(cb);
        float lenab = cv::norm(ab);
        float lencb = cv::norm(cb);
        return std::acos(dot / (lenab + lencb + 1e-6));// 1e-6防止除以0，求得两向量间夹角
    };

    for(size_t i = 0; i < Contours.size(); i++)
    {
        for(size_t j = 1; j < Contours[i].size() - 1; j++)// 从第二个到倒数第二个
        {
            float angle = calAngle(Contours[i][j - 1], Contours[i][j], Contours[i][j + 1]);
            float ratio = CV_PI - angle;// ratio越大，曲率越大
            if(ratio > curve)
                theContours[i].push_back(Contours[i][j]);
        }
    }

    cv::Mat cont = img.clone();// 显示等距后的结果
    for(size_t i = 0; i < theContours.size(); i++)
        cv::drawContours(cont, theContours, i, cv::Scalar(0, 255, 0), 2);
    cv::imshow("theContours2", cont);
    cv::waitKey(0);
}

std::vector<std::vector<cv::Point>> process::gettheCoutours(int type)
{
    if(type)
        gettheCoutours1();
    else
        gettheCoutours2();

    return theContours;
}

void process::hierachytest()
{
    for(const auto &hier : hierarchy)
    {
        for(size_t i = 0; i < 4; i++)
        {
            std::cout << hier[i] << ' ';
        }
        std::cout << std::endl;
    }
}
