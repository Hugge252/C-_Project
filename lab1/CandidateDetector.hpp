#pragma once


#include <opencv2/opencv.hpp>

class CandidateDetector 
{
    public:

    std::vector<std::vector<cv::Point2f>> candidateCorners;
     cv::Mat detect (const cv::Mat& frame);
};