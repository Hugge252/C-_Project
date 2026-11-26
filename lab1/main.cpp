#include <iostream>
#include <opencv2/opencv.hpp>
#include "CandidateDetector.hpp"
#include "MarkerRecognition.hpp"
#include "InteractionMapping.hpp"


int main()
{
    // Open the supplied video file
    cv::VideoCapture video("resources/camera_test.mp4");

    if (!video.isOpened())
    {
        std::cerr << "Could not open the video.\n";
        return 1;
    }

    CandidateDetector detector;
    MarkerRecognition markerRecognition;
    InteractionMapping interactionMapping;

    cv::Mat frame;

    
    video.read(frame);
      cv::Mat framee =  detector.detect(frame);

        
   

    auto dict = markerRecognition.initDictionary(framee, detector, 4, 4);
    std::cout << "Markers in dict: " << dict.markers.size() << std::endl;

   /* for (const auto& marker : dict) {
        std::cout << "Marker ID: " << marker.id

    }*/
        

    while (video.read(frame))
    {
        // Detect and draw marker candidates
        cv::Mat contourImage = detector.detect(frame);

        double lightningThreshold;
        cv::Mat cameraIntrinsics;
        

        cv::Mat markers = markerRecognition.markerRecognition(frame, 0.05f, cameraIntrinsics, dict, detector);
        //cv::Mat rotation = interactionMapping(markers, 4);

        //cv::imshow("OpenCV video test", contourImage);
       cv::imshow("Marker Recognition", markers);
        //cv::imshow("Marker Recognition", rotation);

        if (cv::waitKey(20) == 27)
        {
            break;
        }
    }

    return 0;
}