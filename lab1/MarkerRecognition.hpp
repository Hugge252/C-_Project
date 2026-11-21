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
	

	//cv::Mat markerRecognition(const cv::Mat& inputImage, float markerLegnth);
	cv::Mat markerRecognition(const cv::Mat& inputImage, double lightningThreshold, cv::Mat& cameraIntrinsics, const std::vector<cv::Mat>& knownMarkerPatters);
	cv::Mat binarise(const cv::Mat& inputImage, double lightningTHreshold);
	cv::Point2f locateLineIntersect(const std::vector<cv::Vec4i> lines);
	cv::mat computeMarkerHomography(const std::vector<cv::Point2f> refinedCorners, const float matchOrientation, const float matchSize);


private:

	void detectMarkers(const cv::Mat& outPut, std::vector<std::vector<cv::Point2f>>& markerCorners, std::vector<int>& markerIds, std::vector<std::vector<cv::Point2f>>& rejectedMarkers);
	
	//cv::Mat binaryImage;
	//std::vector<cv::Mat> components; //A vector of components
	//std::vector<cv::Point> contour;
	//std::vector<cv::Vec4f> boundryLines;

	/*cv::aruco::DetectorParameters detectorParams;
	cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_ARUCO_ORIGINAL);
	cv::aruco::ArucoDetector detector{ dictionary, detectorParams };
	cv::Mat cameraMatrix;
	cv::Mat distCoeffs;
	*/
};