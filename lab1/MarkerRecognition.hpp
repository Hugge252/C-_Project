#pragma once
#include <opencv2/opencv.hpp>
#include <list>
#include <CandidateDetector.hpp>
//#include <opencv2/aruco.hpp>
//#include <iostream>
//#include <opencv2/objdetect/aruco_detector.hpp>
//#include <opencv2/geometry.hpp>

class MarkerRecognition {

public:


	//std::vector<int> markerIds; //ID of the markers
	//std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
	
	struct Marker {
		cv::Mat pattern;
		int id;
		double orientation;
		double size;
	};

	struct Dictionary {
		std::vector<Marker> markers;
		int markerRows;
		int markerCols;
	};

	//cv::Mat markerRecognition(const cv::Mat& inputImage, float markerLegnth);
	cv::Mat markerRecognition(const cv::Mat& inputImage, double lightningThreshold, cv::Mat& cameraIntrinsics, const Dictionary& knownMarkerPatters, CandidateDetector& detector);
	cv::Mat binarise(const cv::Mat& inputImage, double lightningTHreshold);
	std::vector<std::vector<cv::Point2f>> locateLineIntersect(const std::vector<cv::Vec4i> lines);
	std::vector<cv::Mat> normalize( std::vector<std::vector<cv::Point2f>>& refinedCorners, const cv::Mat& inputImage);
	std::vector<Marker>  matchWithKnownMarkers(const std::vector<cv::Mat> normPatterns, const Dictionary& knownMarkerPatters);
	std::vector<cv::Mat> decode(const std::vector<cv::Mat>& normilizedMarkers, const Dictionary& dict);
	Dictionary initDictionary(const cv::Mat& inputImage, const CandidateDetector& detector ,int markerRows, int markerCols);




private:

	void detectMarkers(const cv::Mat& outPut, std::vector<std::vector<cv::Point2f>>& markerCorners, std::vector<int>& markerIds, std::vector<std::vector<cv::Point2f>>& rejectedMarkers);
	cv::Mat computeMarkerHomography(const std::vector<cv::Point2f>& refinedCorners, const MarkerRecognition::Marker& markerMatch, const cv::Mat& cameraIntrinsic);
	cv::Mat optimiseTransformation(const cv::Mat& initialPose, const cv::Mat& cameraIntrinsics);
	
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