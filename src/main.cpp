#include <iostream>
#include <opencv2/opencv.hpp>
//#include "process.hpp"
#include "rapid.hpp"

int main()
{
    //process test(10.0f, 1.0f);
    //test.getContours();
    //test.hierachytest();
    //test.gettheCoutours(1);
    //test.gettheCoutours(0);

    rapid test;
    test.getContours();
    //test.gettheCoutours(1);
    //test.gettheCoutours(0);
    test.toTXT();

    return 0;
}
