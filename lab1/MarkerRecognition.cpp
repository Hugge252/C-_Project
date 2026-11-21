
#include "MarkerRecognition.hpp"
#include  <iostream>
#include <list>
#include <CandidateDetector.hpp>
#include <algorithm>

	
//cv::Mat MarkerRecognition::markerRecognition(const cv::Mat& inputImage) {
//
//	
//	//hittar hörnen, id samt rejected markers från videon
//	
//	
//	//Används för debugging
//	/*std::cout << "Antal markers: "
//		<< markerIds.size()
//		<< std::endl;
//
//	std::cout << "Markers: "
//		<< markerIds.size()
//		<< std::endl;
//
//	std::cout << "Rejected: "
//		<< rejectedCandidates.size()
//		<< std::endl;
//
//	std::cout << "Image: "
//		<< inputImage.cols
//		<< " x "
//		<< inputImage.rows
//		<< std::endl;
//
//	for (size_t i = 0; i < markerIds.size(); i++)
//	{
//		std::cout << "Marker ID: "
//			<< markerIds[i]
//			<< std::endl;
//	}*/
//
//
//	cv::Mat output = inputImage.clone();
//
//	
//	
//	float s = 1.0f;
//	// The 4 corners of each of the markers
//	const std::vector<cv::Point3f> objectPoints = {
//
//		{-s / 2,  s / 2, 0}, //Top left
//		{ s / 2,  s / 2, 0}, //Top rigth
//		{ s / 2, -s / 2, 0}, //Bottom right
//		{-s / 2, -s / 2, 0} //Bottom left
//	};
//
//	double w = output.cols; //Width of the image
//	double h = output.rows; //height of the image
//	double f = w; //A guess of the focal length
//
//	// 3X3 matrix from the image
//	cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) <<
//		f, 0, w / 2.0,
//		0, f, h / 2.0,
//		0, 0, 1);
//
//	//Creates a mat matrix with zeros, 5 rows and 1 cols (double)
//	//Used incase the camre has distorion which must be in mind when calculateing rotation and pos
//	//For now the dist is made of zeros which mean no distorion.
//	cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);
//
//	//detects the corner/s, id:s and rejected canditades from output(clone of the input)
//	 detector.detectMarkers(output, markerCorners, markerIds, rejectedCandidates);
//	
//	 
//
//	//Draws the detected corner/s, ids 
//	if (!markerIds.empty()) {
//
//		//Green color for approved
//		cv::aruco::drawDetectedMarkers(
//			output,
//			markerCorners,
//			markerIds,
//			cv::Scalar(0, 255, 0)
//		);
//		//Red color for rejected
//		cv::aruco::drawDetectedMarkers(
//			output,
//			rejectedCandidates,
//			cv::noArray(),
//			cv::Scalar(0, 0, 255)
//		);
//
//		
//		//Loops through the corners
//		for (size_t i = 0; i < markerCorners.size(); i++)
//		{
//			cv::Vec3d midpoint, rotation; //
//			//Calculate midpoint and rotation of the markers
//			cv::solvePnP(objectPoints, markerCorners[i], cameraMatrix,distCoeffs, rotation, midpoint, false, cv::SOLVEPNP_IPPE_SQUARE);
//			//draw the midpoint and rotation from the cameraMatrix 
//			cv::drawFrameAxes(output, cameraMatrix, distCoeffs, rotation, midpoint, s * 0.5f);
//
//			
//		}
//		
//		
//	}
//	
//	
//	
//
//	return output;
//	
//
//}

cv::Mat MarkerRecognition::markerRecognition(const cv::Mat& inputImage, double lightningThreshold, cv::Mat& cameraIntrinsics, const Dictionary& knownMarkerPatters, CandidateDetector& detector) {
	//binarise the image
	std::cout << "New Marekr method running" << std::endl;
	//cv::Mat binaryImage = binarise(inputImage, lightningThreshold);
	////A matris with each pixels connected area
	//cv::Mat labels;
	cv::Mat displayImage = inputImage.clone();
	
	//CandidateDetector detector;
	cv::Mat contoureImage = detector.detect(inputImage);

	auto corners = detector.candidateCorners;

	//Draws the circles before refinement
	for (const auto& rectangel : corners) {
		for (const auto& point : rectangel) {
			cv::circle(displayImage, point, 5, cv::Scalar(0, 0, 255), -1);
		}
	}

	cv::Mat greyScaleImage; 

	cv::cvtColor(inputImage, greyScaleImage, cv::COLOR_BGR2GRAY);

	for (const auto& point : corners) {
		cv::cornerSubPix(
			greyScaleImage, //Need grey image to analyze light shifts
			point, //Saves the refinde corners in the same vektor
			cv::Size(5, 5), //Determed the area to work with
			cv::Size(-1, -1), //Area to ignore, -1,-1 means ingore nothin
			//Break condition. Uses both precision and max itterations. Max 40 itterations and end when changes becomes small
			cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 40, 0.001)

		);

		
	}
	
	//Draws them after refinemnet
	for (const auto& rectangel : corners) {
		for (const auto& point : rectangel) {
			cv::circle(displayImage, point, 5, cv::Scalar(255, 0, 0), -1);
			
			
		}
	}

	auto normilizedPattern = normalize(corners, inputImage);

	auto decodeMarker = decode(normilizedPattern, knownMarkerPatters);

	//Stores the normilized patterns for future compareisong and ID
	

	auto idMarkers = matchWithKnownMarkers(decodeMarker, knownMarkerPatters);
	
	
	//auto id;
	

	
		
	//cv::imshow("ID", idMat);
	//std::cout << "ID" std::cout << id
	

	

	//int components = cv::connectedComponents(binaryImage, labels);  //The total of connections
	//std::list<cv::Mat> recognisedMarkers; //A empty list of the markers

	////loop through all the components in order to find the contours
	//for (int i = 1; i < components; i++) {
	//	cv::Mat component; //Stores the seperate components

	//	//Compare and extracts each component frpm labels whith index, compares eqaul
	//	cv::compare(labels, i, component, cv::CMP_EQ);

	//	//std::vector<std::vector<cv::Point>> contoures; //Stores all the  contoures in a 2D vector of points
	//	//std::vector<cv::Vec4i> hierarchy; //Stores the hieararchy
	//	//cv::findContours(component, contoures, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE); //finds the contours from components and store data in hirearchy and contoure

	//	//std::vector<cv::Point2f> aproximate;

	// //   cv::approxPolyDP(contoures.begin, aproximate, 1, true);
	//	

	//	cv::Mat boundryLines; //Stores the edges
	//	cv::Canny(component, boundryLines, 50, 200); //detects the edges of the contoures
	//	std::vector<cv::Vec4i> lines; //stores the lines in a vec with 4 dimension
	//	cv::HoughLinesP(boundryLines, lines,1, CV_PI / 180, 5, 10, 250); //Finds the lines
	//	
	//	std::vector<std::vector<cv::Point2f>> corners = locateLineIntersect(lines);

	//	if (corners.empty())
	//		continue;

	//	//cv::Mat dislpayCorners = inputImage.clone();

	//	for (const auto& rectangle : corners) {
	//		for (const auto& point : rectangle) {
	//			cv::circle(displayImage, point, 5, cv::Scalar(0, 0, 255), -1);
	//		}
	//	}


	//	//if the lines doesnt form a rectangle, continue to the next component
	//	
	//}
	return displayImage;
	//return recognisedMarkers;
	
}

std::vector<cv::Mat> MarkerRecognition::normalize( std::vector<std::vector<cv::Point2f>>& corners, const cv::Mat& inputImage) {
	
	std::vector<cv::Point2f> srcPoints; //Source points
	std::vector<cv::Point2f> dstPoints; //destination points 
	std::vector<cv::Mat> dst;

	//The dest points 
	auto p1 = cv::Point2f(0, 0);
	auto p2 = cv::Point2f(199, 0);
	auto p3 = cv::Point2f(199, 199);
	auto p4 = cv::Point2f(0, 199);

	dstPoints.push_back(p1);
	dstPoints.push_back(p2);
	dstPoints.push_back(p3);
	dstPoints.push_back(p4);

	for (const auto& rectangel : corners) {
		srcPoints.clear();
		/*auto p1 = rectangel[0];
		auto p2 = rectangel[1];
		auto p3 = rectangel[2];
		auto p4 = rectangel[3];*/
		

		for(int i = 0; i < rectangel.size(); i++)
		srcPoints.push_back(rectangel[i]);

		std::cout << corners.size() << std::endl;
		cv::Mat tempDst;
		
		cv::Mat transformMat = cv::getPerspectiveTransform(srcPoints, dstPoints);
		
		 cv::warpPerspective(inputImage, tempDst, transformMat, cv::Size(200, 200));
		
		 dst.push_back(tempDst);

	}

	return dst;
}

std::vector<MarkerRecognition::Marker>  MarkerRecognition::matchWithKnownMarkers(const std::vector<cv::Mat> normPatterns, const Dictionary& knownMarkerPatters) {
	
	std::vector<Marker> identifiedMarker; //Stores each marker with a id

	
	for (int i = 0; i < normPatterns.size(); i++) {
		auto comparePattern = normPatterns[i]; //the marker to compare to find a match
		double distance = INFINITY; //Distance start at infinity
		int bestId = -1; //id for each marker
		double orientation;
		double size;

		cv::Mat rotate90;
		cv::Mat rotate180;
		cv::Mat rotate270;
		std::vector<cv::Mat> rotations;

		cv::rotate(comparePattern, rotate90, cv::ROTATE_90_CLOCKWISE);
		cv::rotate(comparePattern, rotate180, cv::ROTATE_180);
		cv::rotate(comparePattern, rotate270, cv::ROTATE_90_COUNTERCLOCKWISE);

		//auto dist = cv::norm(comparePattern, knownMarkerPatters[j], cv::NORM_L1); //Calc the difference between 2 patterns
		//auto distRotate90 = cv::norm(rotate90, knownMarkerPatters[j], cv::NORM_L1); //Calc the difference between 2 patterns
		//auto distRotate180 = cv::norm(rotate180, knownMarkerPatters[j], cv::NORM_L1); //Calc the difference between 2 patterns
		//auto distRotate270 = cv::norm(rotate270, knownMarkerPatters[j], cv::NORM_L1); //Calc the difference between 2 patterns

		rotations.push_back(comparePattern);
		rotations.push_back(rotate90);
		rotations.push_back(rotate180);
		rotations.push_back(rotate270);
		for (const auto& marker:knownMarkerPatters.markers) {

			

			for (int k = 0; k < rotations.size(); k++) {

				auto compareRotation = rotations[k];
				//hamming distance represent amount of pixels the 2 markers difference from eachother 
				//Compares decodee markers at a specific roattions with reference markers 
				auto hammingDistance = cv::norm(compareRotation, marker.pattern, cv::NORM_L1); 

				if (hammingDistance < distance) {
					distance = hammingDistance;
					bestId = marker.id;
					orientation = k;

				}

				/*std::cout << "ID: " << j
					<< " Rotation: " << k
					<< " Distance: " << dist << std::endl;*/
			}
			//if a shorter dist if found, update distanse and save the knownMarkerPaterns id 
		
		}


		double averageDistance = distance /
			(comparePattern.rows * comparePattern.cols);

		std::cout << "Best ID: " << bestId
			<< " Orientation: " << orientation
			<< " Distance: " << distance
			<< " Average: " << averageDistance
			<< std::endl;

		identifiedMarker.push_back(Marker{ comparePattern, bestId, orientation, 0.0 });
	}

	return identifiedMarker;

}

std::vector<cv::Mat> MarkerRecognition::decode(const std::vector<cv::Mat>& normilizedMarker, const MarkerRecognition::Dictionary& dict) {

	std::vector<cv::Mat> decodeMarker;

	for (const auto& marker : normilizedMarker) {

		cv::Mat grey;
		cv::cvtColor(marker, grey, cv::COLOR_BGR2GRAY);

		cv::Mat matris(dict.markerRows, dict.markerCols, CV_8UC1);

		//Calculate pixelsize, +2 for border
		auto cellsize = 200 / (dict.markerRows + 2);

		for (int i = 0; i < dict.markerRows; i++) {
			for (int j = 0; j < dict.markerCols; j++) {

				auto x = (j + 1) * cellsize + (cellsize / 2);
				auto y = (i + 1) * cellsize + (cellsize / 2);

				uchar pixelValue = grey.at<uchar>(y, x); //Calculate pixxelvalue for each pixel, using uchar for less memory, 0-255

				if (pixelValue < 128) {
					matris.at<uchar>(i, j) = 0;
				}
				else
					matris.at<uchar>(i, j) = 1;
				
			}
		}

		decodeMarker.push_back(matris);
	}

	return decodeMarker;

}

MarkerRecognition::Dictionary MarkerRecognition::initDictionary(const cv::Mat& inputImage, const CandidateDetector& detector,int markerRows, int markerCols) {
	
	Dictionary dict;
	dict.markerRows = markerRows;
	dict.markerCols = markerCols;
	auto corners = detector.candidateCorners;

	

	cv::Mat greyScaleImage;

	cv::cvtColor(inputImage, greyScaleImage, cv::COLOR_BGR2GRAY);

	for (const auto& point : corners) {
		cv::cornerSubPix(
			greyScaleImage, //Need grey image to analyze light shifts
			point, //Saves the refinde corners in the same vektor
			cv::Size(5, 5), //Determed the area to work with
			cv::Size(-1, -1), //Area to ignore, -1,-1 means ingore nothin
			//Break condition. Uses both precision and max itterations. Max 40 itterations and end when changes becomes small
			cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 40, 0.001)

		);


	}

	

	auto normilizedReference = normalize(corners, inputImage);

	auto decoded = decode(normilizedReference, dict);

	for (int i = 0; i < decoded.size(); i++) {
		dict.markers.push_back(Marker{ decoded[i], i, 0, 0 });
	}

	return dict;
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

std::vector<std::vector<cv::Point2f>> MarkerRecognition::locateLineIntersect(const std::vector<cv::Vec4i> lines) {

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
			double a1 = line1_y2 - line1_y1;
			double b1 = line1_x1 - line1_x2;
			double c1 = a1 * line1_x1 + b1 * line1_y1;

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

	std::vector<std::vector<cv::Point2f>> validPoints;
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

					std::vector<std::pair<cv::Point2f, double>> pairs; //Pairs the point with the angel

					//Creates a object with point and angle and pushes them to the vector for sorting
					pairs.emplace_back(p1, angle1);
					pairs.emplace_back(p2, angle2);
					pairs.emplace_back(p3, angle3);
					pairs.emplace_back(p4, angle4);

					//Decides which order to sort the angels, fropm low to high;
					auto sortOrder = [](const auto& a, const auto& b) {

						if (a.second < b.second)
							return true;
						else
							return false;

						};
						
					// Sorts the vector with angels, from the start to the end, using sortOrder as order
					std::sort(pairs.begin(), pairs.end(), sortOrder);

					std::vector<cv::Point2f> sortedPairs;

					for (int i = 0; i < pairs.size(); i++) {

						auto point = pairs[i].first;
						sortedPairs.push_back(point);
						

					}

					//Checks if the 4 points are convex and have a area within the limit to se if its a valid marker or not
					//if it is, store it in a seperate vector and retunr it.
					if (cv::isContourConvex(sortedPairs)) {
						
						auto area = cv::contourArea(sortedPairs, false);

						if (area >= 2000 && area <= 150000) {

							validPoints.push_back(sortedPairs);
						}
							


					}
					
					
				}
			}
		}
		
		

		
		
	}

	return validPoints;
	

}


	


	/*cv::Mat outputImage = inputImage.clone();
	cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);*/


