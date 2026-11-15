#include <iostream>
#include <opencv2/opencv.hpp>
#include "CandidateDetector.hpp"
#include "MarkerRecognition.hpp"


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
    MarkerRecognition markerRecognition;

    cv::Mat frame;

    while (video.read(frame))
    {
        // Detect and draw marker candidates
        cv::Mat contourImage = detector.detect(frame);

        cv::Mat markers = markerRecognition.markerRecognition(frame, 0.05f);

        cv::imshow("OpenCV video test", contourImage);
        cv::imshow("Marker Recognition", markers);

        if (cv::waitKey(20) == 27)
        {
            break;
        }
    }

    return 0;
}