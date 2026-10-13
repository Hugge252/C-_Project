#pragma once


#include <opencv2/opencv.hpp>

class CandidateDetector 
{
    public:

    std::vector<std::vector<cv::Point>> candidateCorners;
     cv::Mat detect (const cv::Mat& frame);
};