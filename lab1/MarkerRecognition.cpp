#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>
#include <iostream>
#include <opencv2/objdetect/aruco_detector.hpp>
class MarkerRecon
{
public:


	std::vector<int> markerIds; //ID of the markers
	std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandodates; // List of the detected corners, rejected and accepted
	
	cv::Mat markerRecognition(const cv::Mat& inputImage) {
		detector.detectMarkers(inputImage, markerCorners, markerIds);

		cv::Mat outputImage = inputImage.clone();
		if (!markerIds.empty())
			cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);
		return outputImage;
			
		
	}

	cv::Mat outputImage = inputImage.clone();
	cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);


private:
	cv::aruco::DetectorParameters detectorParams;
	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_6X6_250);
	cv::aruco::ArucoDetector detector{ dictionary, detectorParams };
};