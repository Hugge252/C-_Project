#include <iostream>
#include <opencv2/opencv.hpp>
<<<<<<< HEAD
#include "MarkerRecognition.h"
int main()
{
	
	// Open the supplied video file.
	cv::VideoCapture video("resources/camera_test.mp4");
	if (!video.isOpened()) {
		std::cerr << "Could not open the video.\n";
		return 1;
	}


	MarkerRecognition markerRecognition;
	cv::Mat frame;
	cv::Mat gray;
	cv::Mat binary;
=======
#include "CandidateDetector.hpp"
#include "MarkerRecognition.hpp"
int main()
{
    // Open the supplied video file
    cv::VideoCapture video("resources/camera_test_fixed.mp4");
>>>>>>> jens-branch

    if (!video.isOpened())
    {
        std::cerr << "Could not open the video.\n";
        return 1;
    }

    // Create candidate detector
    CandidateDetector detector;

    cv::Mat frame;

    // Read and process one frame at a time
    while (video.read(frame))
    {
        // Detect and draw marker candidates
        cv::Mat contourImage = detector.detect(frame);

        // Display the processed frame
        cv::imshow("OpenCV video test", contourImage);

        // Escape closes the program
        if (cv::waitKey(20) == 27)
        {
            break;
        }
    }

<<<<<<< HEAD
		//Find contours in the binary image, including nested contours
		cv::findContours(binary, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);


		cv::Mat contourImage = frame.clone();
		cv::Mat markers = markerRecognition.markerRecognition(frame, 0.05f);
		
		//cv::Mat markerRecon = markerRecognition.markerRecognition(frame);

		//Check each detected contour.
		for(const auto& contour : contours)
		{
			double contourArea = cv::contourArea(contour);

			//Ignore conoturs that are too small
			if(contourArea > minContourArea)
			{
				std::vector<cv::Point> approximate;

				//Simplify the contour to find its main corner points.
				double perimeter = cv::arcLength(contour, true);
				double epsilon = epsilonFactor * perimeter;

				cv::approxPolyDP(contour, approximate, epsilon, true);

				//A marker candidate should have four corners and be convex
				if(approximate.size() == 4 && cv::isContourConvex(approximate))
				{
					cv::Rect boundingBox = cv::boundingRect(approximate);

					//Compare width and height to check if the shape is roughly square.
					double aspectRatio = static_cast<double>(boundingBox.width) / boundingBox.height;

					if(aspectRatio >= minAspectRatio && aspectRatio <= maxAspectRatio)
					{
						//Draw accepted candidates in green
						std::vector<std::vector<cv::Point>> filteredContour = {contour};

						cv::drawContours(contourImage, filteredContour, -1, 
						cv::Scalar(blueValue, greenValue, redValue));
					}

				}
				else
				{
					//Draw rejected shape in red for debugging.
					std::vector<std::vector<cv::Point>> rejectedContour = {contour};

					cv::drawContours(contourImage, rejectedContour, -1,
					cv::Scalar(rejectedBlueValue, rejectedGreenValue, rejectedRedValue));

				}
				
			}

		}





		//cv::drawContours(contourImage, contours, -1, cv::Scalar(blueValue, greenValue, redValue));

        cv::imshow("Markers", markers);
		//cv::imshow("OpenCV video test", contourImage);

		
		// Wait briefly; Escape closes the program.
		if (cv::waitKey(20) == 27) 
			break;
		}

		
	}
	return 0;
=======
    return 0;
>>>>>>> jens-branch
}