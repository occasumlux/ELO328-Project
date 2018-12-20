#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>

cv::Mat processVeins(cv::Mat& img) {
    if (img.empty()){

        std::cerr << "Image is empty!!!" << std::endl;
        return cv::Mat();
    }
    cv::Mat gray, noNoise, opened, YUV, equalized, final;
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);
    cv::fastNlMeansDenoising(gray, noNoise);
    int kernel_size = 7;
    cv::Mat kernel = cv::Mat::ones(kernel_size, kernel_size, CV_8U);
    cv::morphologyEx(noNoise, opened, cv::MORPH_OPEN, kernel);

    cv::equalizeHist(opened, equalized);
    equalized = 255 - equalized;

    //cv::imshow("Orig", img);
    //cv::imshow("Eq", equalized);
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

    //cv::imshow("Veins", veins);
    cv::threshold(veins, veins, 5, 255, cv::THRESH_BINARY);
    //cv::imshow("Veins Thresh", veins);
    cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
    cv::morphologyEx(veins, veins, cv::MORPH_ERODE, kernel3);
    cv::morphologyEx(veins, veins, cv::MORPH_DILATE, kernel3);
    //cv::imshow("Veins Last Morph", veins);
    //cv::waitKey();
    return veins;
}

int main(int argc, char *argv[])
{
    //cv::Mat img = cv::imread("/home/daryl/University/2018_2/ELO328/ELO328-Project/Veins/veins.png", cv::IMREAD_COLOR);
    cv::Mat cut1 = cv::imread("/home/daryl/University/2018_2/ELO328/ELO328-Project/recorte1.png", cv::IMREAD_COLOR);
    cv::Mat cut2 = cv::imread("/home/daryl/University/2018_2/ELO328/ELO328-Project/recorte2.png", cv::IMREAD_COLOR);
    cv::Mat veins1 = processVeins(cut1);
    cv::Mat veins2 = processVeins(cut2);

    cv::imshow("Cut 1", veins1);
    cv::imshow("Cut 2",veins2);

    //cv::Mat templateRes;
    //cv::Mat resized;
    //cv::resize(veins1,resized, veins2.size());
    //cv::matchTemplate(resized, veins2, templateRes, cv::TM_CCOEFF_NORMED);

    //double minVal, maxVal;
    //cv::Point minLoc, maxLoc;
    //cv::minMaxLoc(templateRes, &minVal, &maxVal, &minLoc, &maxLoc);
    //std::cout << "Min: " << minVal << "Max: " << maxVal << std::endl;
    //cv::namedWindow("Template Result", cv::WINDOW_GUI_EXPANDED);
    //cv::imshow("Template Result", templateRes);

    //cv::imshow("Resized", resized);

    cv::Ptr<cv::xfeatures2d::SURF> detector = cv::xfeatures2d::SURF::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv::Mat descriptors1, descriptors2;
    detector->detectAndCompute( veins1, cv::noArray(), keypoints1, descriptors1 );
    detector->detectAndCompute( veins2, cv::noArray(), keypoints2, descriptors2 );
    cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create(cv::DescriptorMatcher::FLANNBASED);
    std::vector< std::vector<cv::DMatch> > knn_matches;
    matcher->knnMatch( descriptors1, descriptors2, knn_matches, 2 );

    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.9f;
    std::vector<cv::DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    std::cout << good_matches.size() << std::endl;
    //-- Draw matches
    cv::Mat img_matches;
    cv::drawMatches( veins1, keypoints1, veins2, keypoints2, good_matches, img_matches, cv::Scalar::all(-1),
                 cv::Scalar::all(-1), std::vector<char>(), cv::DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );
    //-- Show detected matches
    cv::namedWindow("Good Matches", cv::WINDOW_GUI_EXPANDED);
    imshow("Good Matches", img_matches );

    cv::waitKey();
    /*
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
    cv::Mat kernel3 = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(3,3));
    cv::morphologyEx(veins, veins, cv::MORPH_ERODE, kernel3);
    cv::morphologyEx(veins, veins, cv::MORPH_DILATE, kernel3);
    cv::imshow("Veins Last Morph", veins);
    cv::waitKey();
    */
}
