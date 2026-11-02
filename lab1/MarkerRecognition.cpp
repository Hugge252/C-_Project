
#include "MarkerRecognition.hpp"
#include  <iostream>

	
cv::Mat MarkerRecognition::markerRecognition(const cv::Mat& inputImage, float markerLegnth) 
{

	//Check if the inpit image is empty
	if(inputImage.empty())
	{
		std::cerr << "Error: Input image is empty!" << std::endl;

		markerIds.clear();
		markerCorners.clear();
		rejectedCandidates.clear();

		return cv::Mat();
	}

	
	//hittar hörnen, id samt rejected markers från videon
	detector.detectMarkers(inputImage, markerCorners, markerIds, rejectedCandidates);

	//Används för debugging
	std::cout << "Antal markers: "
		<< markerIds.size()
		<< std::endl;

	std::cout << "Markers: "
		<< markerIds.size()
		<< std::endl;

	std::cout << "Rejected: "
		<< rejectedCandidates.size()
		<< std::endl;

	std::cout << "Image: "
		<< inputImage.cols
		<< " x "
		<< inputImage.rows
		<< std::endl;

	for (size_t i = 0; i < markerIds.size(); i++)
	{
		std::cout << "Marker ID: "
			<< markerIds[i]
			<< std::endl;
	}


	cv::Mat output = inputImage.clone();

	//Om det finns id:s, rita ut hörnen, ids samt färg
	if (!markerIds.empty()) {

		cv::aruco::drawDetectedMarkers(
			output,
			markerCorners,
			markerIds,
			cv::Scalar(0, 255, 0)
		);
	}

	return output;
	

}


	


	/*cv::Mat outputImage = inputImage.clone();
	cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);*/


