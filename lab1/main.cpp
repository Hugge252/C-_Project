#include <iostream>
#include <opencv2/opencv.hpp>
#include "CandidateDetector.hpp"

int main()
{
    // Open the supplied video file
    cv::VideoCapture video("resources/camera_test_fixed.mp4");

    if (!video.isOpened())
    {
        std::cerr << "Could not open the video.\n";
        return 1;
    }

    CandidateDetector detector;
    cv::Mat frame;

    while (video.read(frame))
    {
        // Detect and draw marker candidates
        cv::Mat contourImage = detector.detect(frame);

        cv::imshow("OpenCV video test", contourImage);

        if (cv::waitKey(20) == 27)
        {
            break;
        }
    }

    return 0;
}