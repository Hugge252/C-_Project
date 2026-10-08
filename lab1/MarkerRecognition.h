#pragma once
#include <opencv2/opencv.hpp>
//#include <opencv2/aruco.hpp>
//#include <iostream>
//#include <opencv2/objdetect/aruco_detector.hpp>
//#include <opencv2/geometry.hpp>

class MarkerRecognition {

public:


	std::vector<int> markerIds; //ID of the markers
	std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
	

	cv::Mat markerRecognition(const cv::Mat& inputImage, float markerLegnth);



private:

	cv::QRCodeDetectorAruco qr;

	cv::aruco::DetectorParameters detectorParams;
	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);
	cv::aruco::ArucoDetector detector{ dictionary, detectorParams };
};