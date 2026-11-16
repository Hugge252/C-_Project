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
	cv::Mat outPut;
	cv::Mat greyScaleOutput;
	cv::Mat thresholdOutput;

	

	cv::Mat markerRecognition(const cv::Mat& inputImage, double& lightningThreshold, cv::Mat& cameraIntrinsics, std::vector& kownMarkerPatterns);
	
	



private:

	void detectMarkers(const cv::Mat& outPut, std::vector<std::vector<cv::Point2f>>& markerCorners, std::vector<int>& markerIds, std::vector<std::vector<cv::Point2f>>& rejectedMarkers);
	
	//cv::Mat binaryImage;
	//std::vector<cv::Mat> components; //A vector of components
	//std::vector<cv::Point> contour;
	//std::vector<cv::Vec4f> boundryLines;

	////Contains X and Y cordinates
	//std::vector<cv::Point2f> corners; //A vector of the 4 corners
	//std::vector<cv::Point2f> refinedCorners;



	//cv::aruco::DetectorParameters detectorParams;
	//cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);
	//cv::aruco::ArucoDetector detector{ dictionary, detectorParams };
	//cv::Mat cameraMatrix;
	//cv::Mat distCoeffs;
	//
};