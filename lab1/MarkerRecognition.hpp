#pragma once
#include <opencv2/opencv.hpp>
//#include <opencv2/aruco.hpp>
//#include <iostream>
//#include <opencv2/objdetect/aruco_detector.hpp>
//#include <opencv2/geometry.hpp>

class MarkerRecognition {

public:


	//std::vector<int> markerIds; //ID of the markers
	//std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
	

	//cv::Mat markerRecognition(const cv::Mat& inputImage, float markerLegnth);
	cv::Mat markerRecognition(const cv::Mat& inputImage, double lightningThreshold, cv::Mat& cameraIntrinsics, const std::vector<cv::Mat>& knownMarkerPatters);
	cv::Mat binarise(const cv::Mat& inputImage, double lightningTHreshold);
	 locateLineIntersect(const std::vector<cv::Vec4i> lines);



private:

	

	/*cv::aruco::DetectorParameters detectorParams;
	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);
	cv::aruco::ArucoDetector detector{ dictionary, detectorParams };
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;*/
	
};