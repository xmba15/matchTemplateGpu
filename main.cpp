﻿#include "matchTemplateCpu.h"
#include "matchTemplateGpu.cuh"
#include "utility.h"

#include <iostream>

int main(int argc, char *argv[])
{
    const int loop_num = 5;
    double time;

    cv::Mat img(sz1080p, CV_8UC1, cv::Scalar(0));
    cv::Mat templ(cv::Size(32, 32), CV_8UC1, cv::Scalar(255));
    cv::Size corrSize(img.cols - templ.cols + 1, img.rows - templ.rows + 1);
    cv::Mat result(corrSize, CV_32FC1, cv::Scalar(0.0f));
    cv::Mat result_cv(corrSize, CV_32FC1, cv::Scalar(0.0f));

#ifdef VALIDATION

    // Naive Implementation
    time = launchMatchTemplateCpu(img, templ, result, loop_num);
    std::cout << "Naive: " << time << " ms." << std::endl;

    // OpenCV
    time = launchMatchTemplateCV(img, templ, result_cv, loop_num);
    std::cout << "OpenCV: " << time << " ms." << std::endl;

#endif

    cv::gpu::GpuMat d_img(img);
    cv::gpu::GpuMat d_templ(templ);
    cv::gpu::GpuMat d_result(corrSize, CV_32FC1, cv::Scalar(0.0f));
    cv::gpu::GpuMat d_result2(corrSize, CV_32FC1, cv::Scalar(0.0f));
    cv::gpu::GpuMat d_result3(corrSize, CV_32FC1, cv::Scalar(0.0f));

    // CUDA Implementation
    time = launchMatchTemplateGpu(d_img, d_templ, d_result, loop_num);
    std::cout << "CUDA: " << time << " ms." << std::endl;

    // CUDA Implementation (static shared memory)
    time = launchMatchTemplateGpu_withStaticSharedMemory(d_img, d_templ, d_result2, loop_num);
    std::cout << "CUDA(withStaticSharedMemory): " << time << " ms." << std::endl;

    std::cout << std::endl;

#ifdef VALIDATION

    // Verification
    verify(result, d_result);
    verify(result, d_result2);

#endif

    return 0;
}
