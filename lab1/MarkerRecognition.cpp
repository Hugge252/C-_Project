
#include "MarkerRecognition.hpp"
#include  <iostream>
#include <list>

	
cv::Mat MarkerRecognition::markerRecognition(const cv::Mat& inputImage) {

	
	//hittar hörnen, id samt rejected markers från videon
	
	
	//Används för debugging
	/*std::cout << "Antal markers: "
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
	}*/


	cv::Mat output = inputImage.clone();

	
	
	float s = 1.0f;
	// The 4 corners of each of the markers
	const std::vector<cv::Point3f> objectPoints = {

		{-s / 2,  s / 2, 0}, //Top left
		{ s / 2,  s / 2, 0}, //Top rigth
		{ s / 2, -s / 2, 0}, //Bottom right
		{-s / 2, -s / 2, 0} //Bottom left
	};

	double w = output.cols; //Width of the image
	double h = output.rows; //height of the image
	double f = w; //A guess of the focal length

	// 3X3 matrix from the image
	cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) <<
		f, 0, w / 2.0,
		0, f, h / 2.0,
		0, 0, 1);

	//Creates a mat matrix with zeros, 5 rows and 1 cols (double)
	//Used incase the camre has distorion which must be in mind when calculateing rotation and pos
	//For now the dist is made of zeros which mean no distorion.
	cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);

	//detects the corner/s, id:s and rejected canditades from output(clone of the input)
	 detector.detectMarkers(output, markerCorners, markerIds, rejectedCandidates);
	
	 

	//Draws the detected corner/s, ids 
	if (!markerIds.empty()) {

		//Green color for approved
		cv::aruco::drawDetectedMarkers(
			output,
			markerCorners,
			markerIds,
			cv::Scalar(0, 255, 0)
		);
		//Red color for rejected
		cv::aruco::drawDetectedMarkers(
			output,
			rejectedCandidates,
			cv::noArray(),
			cv::Scalar(0, 0, 255)
		);

		
		//Loops through the corners
		for (size_t i = 0; i < markerCorners.size(); i++)
		{
			cv::Vec3d midpoint, rotation; //
			//Calculate midpoint and rotation of the markers
			cv::solvePnP(objectPoints, markerCorners[i], cameraMatrix,distCoeffs, rotation, midpoint, false, cv::SOLVEPNP_IPPE_SQUARE);
			//draw the midpoint and rotation from the cameraMatrix 
			cv::drawFrameAxes(output, cameraMatrix, distCoeffs, rotation, midpoint, s * 0.5f);

			
		}
		
		
	}
	
	
	

	return output;
	

}

cv::Mat MarkerRecognition::markerRecognition(const cv::Mat& inputImage, double lightningThreshold, cv::Mat& cameraIntrinsics, const std::vector<cv::Mat>& knownMarkerPatters) {
	//binarise the image
	cv::Mat binaryImage = binarise(inputImage, lightningThreshold);
	//A matris with each pixels connected area
	cv::Mat labels;
	
	int components = cv::connectedComponents(binaryImage, labels);  //The total of connections
	std::list<cv::Mat> recognisedMarkers; //A empty list of the markers

	//loop through all the components in order to find the contours
	for (int i = 1; i < components; i++) {
		cv::Mat component; //Stores the seperate components

		//Compare and extracts each component frpm labels whith index, compares eqaul
		cv::compare(labels, i, component, cv::CMP_EQ);

		std::vector<std::vector<cv::Point>> contoures; //Stores all the  contoures in a 2D vector of points
		std::vector<cv::Vec4i> hierarchy; //Stores the hieararchy
		 cv::findContours(component, contoures, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE); //finds the contours from components and store data in hirearchy and contoure

		cv::Mat boundryLines; //Stores the edges
		cv::Canny(component, boundryLines, 50, 200); //detects the edges of the contoures
		std::vector<cv::Vec4i> lines; //stores the lines in a vec with 4 dimension
		cv::HoughLinesP(boundryLines, lines,1, CV_PI / 180, 5, 10, 250); //Finds the lines
		
		//if the lines doesnt form a rectangle, continue to the next component
		if (lines.size() != 4)
			continue;
	}

	return recognisedMarkers;
	
}

cv::Mat MarkerRecognition::binarise(const cv::Mat& inputImage, double lightningThreshold) {
	cv::Mat binaryImage;
	cv::Mat greyImage;
	//Convertrs the image to grey in order to calculate threshold
	cv::cvtColor(inputImage, greyImage, cv::COLOR_BGR2GRAY);
	//The method biniraise the input imgae from inputImage, threshold, value for the pixels and binary threshold
	cv::threshold(greyImage, binaryImage, lightningThreshold, 225, cv::THRESH_BINARY);

	return binaryImage;
}

MarkerRecognition::locateLineIntersect(const std::vector<cv::Vec4i> lines) {
	
}


	


	/*cv::Mat outputImage = inputImage.clone();
	cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);*/


