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

    //修改添加，高斯滤波
    //cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);// 不行，效果不好

    // 二值化
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
    pointNum = 0;
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
                pointNum++;
            }
        }
        pointNum++;// 由于每个轮廓的第一个点默认加入，所以这里也要++
    }
    cv::Mat cont = img.clone();// 显示等距后的结果
    for(size_t i = 0; i < theContours.size(); i++)
        cv::drawContours(cont, theContours, i, cv::Scalar(255, 0, 0), 2);
    cv::imshow("theContours1", cont);
    cv::waitKey(0);
}

// AI建议滑动窗口处理
// std::vector<cv::Point2f> smoothContour(const std::vector<cv::Point>& contour, int windowSize = 5) {
//     std::vector<cv::Point2f> smoothed;
//     int half = windowSize / 2;
//     for (size_t i = 0; i < contour.size(); ++i) {
//         float sumX = 0, sumY = 0;
//         int count = 0;
//         for (int j = -half; j <= half; ++j) {
//             int idx = i + j;
//             if (idx >= 0 && idx < contour.size()) {
//                 sumX += contour[idx].x;
//                 sumY += contour[idx].y;
//                 count++;
//             }
//         }
//         smoothed.push_back(cv::Point2f(sumX / count, sumY / count));
//     }
//     return smoothed;
// }

void process::gettheCoutours2()// 曲率特征
{
    pointNum = 0;
    theContours.clear();// 注意先清零
    theContours.resize(Contours.size());// 若要使用索引访问则先要分配空间

    // // 匿名函数，获得夹角
    // auto calAngle = [](cv::Point2f a, cv::Point2f b, cv::Point2f c)
    // {
    //     cv::Point2f ab = b - a;
    //     cv::Point2f bc = c - b;// 相对的向量 //现在改回bc正向量
    //     // float dot = ab.dot(bc);
    //     // float lenab = cv::norm(ab);
    //     // float lenbc = cv::norm(bc);
    //     float dot = ab.x * bc.x + ab.y * bc.y;// 改成这种试一下
    //     float lenab = std::sqrt(ab.x * ab.x + ab.y * ab.y);
    //     float lenbc = std::sqrt(bc.x * bc.x + bc.y * bc.y);
    //     // 一开始这里写成lenab + lencb，导致取的点好像不是很对
    //     //return std::acos(dot / (lenab * lencb + 1e-6));// 1e-6防止除以0，求得两向量间夹角
    //     float cos_theta = dot / (lenab * lenbc + 1e-6f);
    //     cos_theta = std::max(-1.0f, std::min(1.0f, cos_theta)); // 防止数值溢出
    //     return std::acos(cos_theta);
    // };

    for(size_t i = 0; i < Contours.size(); i++)
    {

        //std::vector<cv::Point2f> smoothPts = smoothContour(Contours[i]);// 滑动窗口平滑处理 //好像不是问题所在

        // 亚像素精度测试 // 终于搞好了，就是像素点的问题
        std::vector<cv::Point2f> contour_points_float;
        for (const auto& pt : Contours[i]) {
            contour_points_float.push_back(cv::Point2f(pt.x, pt.y));
        }
        cv::cornerSubPix(binary, contour_points_float, cv::Size(5,5), cv::Size(-1,-1),
                        cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 40, 0.001));


        if (contour_points_float.size() < 3) continue; // 防止越界
        for(size_t j = 1; j < contour_points_float.size() - 1; j++)// 从第二个到倒数第二个
        {
            float angle = calAngle(contour_points_float[j - 1], contour_points_float[j], contour_points_float[j + 1]);
            //float ratio = CV_PI - angle;// ratio越大，曲率越大
            std::cout << "ANGLE" << pointNum << ":" << angle << std::endl;
            std::cout << "POS" << contour_points_float[j] << std::endl;
            if(angle < curve)
            {
                theContours[i].push_back(contour_points_float[j]);
                pointNum++;
            }
        }
    }

    cv::Mat cont = img.clone();// 显示曲率特征后的结果
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

float process::calAngle(cv::Point2f a, cv::Point2f b, cv::Point2f c)
{
        // cv::Point2f ab = b - a;
        // cv::Point2f bc = c - b;// 相对的向量 //现在改回bc正向量
        // // float dot = ab.dot(bc);
        // // float lenab = cv::norm(ab);
        // // float lenbc = cv::norm(bc);
        // float dot = ab.x * bc.x + ab.y * bc.y;// 改成这种试一下
        // float lenab = std::sqrt(ab.x * ab.x + ab.y * ab.y);
        // float lenbc = std::sqrt(bc.x * bc.x + bc.y * bc.y);
        // // 一开始这里写成lenab + lencb，导致取的点好像不是很对
        // //return std::acos(dot / (lenab * lencb + 1e-6));// 1e-6防止除以0，求得两向量间夹角
        // float cos_theta = dot / (lenab * lenbc + 1e-6f);
        // cos_theta = std::max(-1.0f, std::min(1.0f, cos_theta)); // 防止数值溢出
        // return std::acos(cos_theta);

        // 计算角度的新方法试一下
        cv::Point2f u = b - a;
        cv::Point2f v = c - b;
        float dot = u.x * v.x + u.y * v.y;
        float cross = u.x * v.y - u.y * v.x;
        float angle = std::atan2(std::abs(cross), dot); // 返回0~π范围的角度
        return angle;
}
