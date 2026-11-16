
#include "MarkerRecognition.hpp"
#include  <iostream>
#include <list>
#include <CandidateDetector.hpp>

	
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
	
	
	cv::Mat contoureImage = CandidateDetector::detect(inputImage);
	int components = cv::connectedComponents(contoureImage, labels);  //The total of connections
	std::list<cv::Mat> recognisedMarkers; //A empty list of the markers

	//loop through all the components in order to find the contours
	for (int i = 1; i < components; i++) {
		cv::Mat component; //Stores the seperate components

		//Compare and extracts each component frpm labels whith index, compares eqaul
		cv::compare(labels, i, component, cv::CMP_EQ);

		//std::vector<std::vector<cv::Point>> contoures; //Stores all the  contoures in a 2D vector of points
		//std::vector<cv::Vec4i> hierarchy; //Stores the hieararchy
		// cv::findContours(component, contoures, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE); //finds the contours from components and store data in hirearchy and contoure

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

cv::Point2f MarkerRecognition::locateLineIntersect(const std::vector<cv::Vec4i> lines) {

	std::vector<cv::Point2f> intersectingPoints; //Stores all the intersecting points

	// Find 4 intersecting lines and its points. Results in 6 points
	for (int i = 0; i < 4; i++) {
		for (int j = i + 1; j < 4; j++) {

			//The start and end points for 2 lines to check intersections
			auto line1_x1 = lines[i][0];
			auto line1_y1 = lines[i][1];
			auto line1_x2 = lines[i][2];
			auto line1_y2 = lines[i][3];

			auto line2_x1 = lines[j][0];
			auto line2_y1 = lines[j][1];
			auto line2_x2 = lines[j][2];
			auto line2_y2 = lines[j][3];

			//Each lines ekvation
			int a1 = line1_y2 - line1_y1;
			int b1 = line1_x1 - line1_x2;
			int c1 = a1 * line1_x1 + b1 * line1_y1;

			double a2 = line2_y2 - line2_y1;
			double b2 = line2_x1 - line2_x2;
			double c2 = a2 * line2_x1 + b2 * line2_y1;

			double D = a1 * b2 - a2 * b1; // Calculates the determinate

			// If D isnt 0, the lines are intersecting 
			if (D != 0) {
				//Calculates the cordinates for the point where the 2 lines intersect
				double x = (c1 * b2 - c2 * b1) / D;
				double y = (a1 * c2 - a2 * c1) / D;

				intersectingPoints.push_back(cv::Point2f(x, y)); //Add the intersecting point in the total point vector
				
			}
		}
	}

	//Extracts 4 points to validate valid 4 points. Using 
	for (int i = 0; i < intersectingPoints.size(); i++) {

		auto p1 = intersectingPoints[i];
		for (int j = i + 1; j < intersectingPoints.size(); j++) {

			auto p2 = intersectingPoints[j];

			for (int k = j + 1; k < intersectingPoints.size(); k++) {

				auto p3 = intersectingPoints[k];

				for (int l = k + 1; l < intersectingPoints.size(); l++) {

					auto p4 = intersectingPoints[l];
					//Calculate the centrum from the points
					auto centerX = (p1.x + p2.x + p3.x + p4.x) / 4;
					auto centerY = (p1.y + p2.y + p3.y + p4.y) / 4;

					double angle1 = atan2(p1.y - centerY, p1.x - centerX);
					double angle2 = atan2(p2.y - centerY, p2.x - centerX);
					double angle3 = atan2(p3.y - centerY, p3.x - centerX);
					double angle4 = atan2(p4.y - centerY, p4.x - centerX);
				}
			}
		}
		
		
		
		
	}
	

}


	


	/*cv::Mat outputImage = inputImage.clone();
	cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);*/


