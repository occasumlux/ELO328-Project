#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;

int main(int argc, char *argv[])
{
    cv::Mat img = cv::imread("/home/daryl/University/2018_2/ELO328/Veins/veins.png", cv::IMREAD_COLOR);
    if (img.empty()){
    
        std::cerr << "Image not found!!!" << std::endl;
        return -1;
    }
    cv::Mat gray, noNoise, opened, YUV, equalized, final;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::fastNlMeansDenoising(gray, noNoise);
    int kernel_size = 7;
    cv::Mat kernel = cv::Mat::ones(kernel_size, kernel_size, CV_8U);
    cv::morphologyEx(noNoise, opened, cv::MORPH_OPEN, kernel);

    cv::equalizeHist(opened, equalized);
    equalized = 255 - equalized;

    cv::imshow("Orig", img);
    cv::imshow("Eq", equalized);
    //cv::waitKey();

    cv::Mat clone = equalized.clone();
    cv::Mat veins = clone.clone();
    veins = 0;
    cv::Mat kernel2 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(5,5));

    for(;;) {
        cv::Mat temp, eroded;
        cv::morphologyEx(clone, eroded, cv::MORPH_ERODE, kernel2);
        cv::morphologyEx(eroded, temp, cv::MORPH_DILATE, kernel2);
        cv::subtract(clone, temp, temp);
        cv::bitwise_or(veins, temp, veins);
        clone = eroded.clone();
        //cv::imshow("Veins", eroded);
        //cv::waitKey(10);
        //std::cout << "Uff" << std::endl;
        if (cv::countNonZero(clone) == 0)
            break;
    }
    cv::imshow("Veins", veins);
    cv::threshold(veins, veins, 5, 255, cv::THRESH_BINARY);
    cv::imshow("Veins Thresh", veins);
    cv::waitKey();
}
