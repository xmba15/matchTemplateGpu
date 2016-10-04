﻿#include "matchTemplateCpu.h"
#include "matchTemplateGpu.cuh"
#include "utility.h"

#include <iostream>

int main(int argc, char *argv[])
{
    const int loop_num = 5;

    cv::Mat img(sz1080p, CV_8UC1, cv::Scalar(0));
    cv::Mat templ(cv::Size(32, 32), CV_8UC1, cv::Scalar(255));
    cv::Size corrSize(img.cols - templ.cols + 1, img.rows - templ.rows + 1);
    cv::Mat result(corrSize, CV_32FC1, cv::Scalar(0.0f));
    cv::Mat result_cv(corrSize, CV_32FC1, cv::Scalar(0.0f));

    // Naive Implementation
    double time = launchMatchTemplateCpu(img, templ, result, loop_num);
    std::cout << "Naive: " << time << " ms." << std::endl;

    // OpenCV
    time = launchMatchTemplateCV(img, templ, result_cv, loop_num);
    std::cout << "OpenCV: " << time << " ms." << std::endl;

    std::cout << std::endl;

    return 0;
}
