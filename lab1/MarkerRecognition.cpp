
#include "MarkerRecognition.hpp"
#include  <iostream>
#include <list>
#include <CandidateDetector.hpp>
#include <algorithm>

	


cv::Mat MarkerRecognition::markerRecognition(const cv::Mat& inputImage, double lightningThreshold, cv::Mat& cameraIntrinsics, const Dictionary& knownMarkerPatters, CandidateDetector& detector) {
	
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

	
	

	
	return recognisedMarkers;
	
}

std::vector<cv::Mat> MarkerRecognition::normalize( std::vector<std::vector<cv::Point2f>>& corners, const cv::Mat& inputImage) {
	
	std::vector<cv::Point2f> srcPoints; //Source points
	std::vector<cv::Point2f> dstPoints; //destination points 
	std::vector<cv::Mat> dst;

	//The dest points. Used to nomrilize the markers to 200*200 pixels
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
		
		//Transform the marker acording to the transformation matrix
		 cv::warpPerspective(inputImage, tempDst, transformMat, cv::Size(200, 200));
		
		 //Add it to the vector conatining normilised markers
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

		//Rotates the compare pattern and store it in a vector to later get the current rotation when comparing
		//I rotate because i dont know which angle the camera has read the marker
		cv::rotate(comparePattern, rotate90, cv::ROTATE_90_CLOCKWISE);
		cv::rotate(comparePattern, rotate180, cv::ROTATE_180);
		cv::rotate(comparePattern, rotate270, cv::ROTATE_90_COUNTERCLOCKWISE);

		rotations.push_back(comparePattern);
		rotations.push_back(rotate90);
		rotations.push_back(rotate180);
		rotations.push_back(rotate270);
		//Extract each marker from the dictionary
		for (const auto& marker:knownMarkerPatters.markers) {

			

			for (int k = 0; k < rotations.size(); k++) {

				//std::cout << "Rotations: " << rotations[k] << std::endl;

				auto compareRotation = rotations[k];
				//hamming distance represent amount of pixels the 2 markers difference from eachother 
				//Compares decodee markers at a specific roattions with reference markers 
				//NORM_L1 sums the totalt difference in pixel value
				auto hammingDistance = cv::norm(compareRotation, marker.pattern, cv::NORM_L1); 

				//if the allowed hamming is smaller then 5, the program crash. Dont change for know!!
				if (hammingDistance >= 5)
					continue;

				if (hammingDistance < distance) {
					distance = hammingDistance;
					bestId = marker.id;
					size = marker.size;
					orientation = k; //Which orientation gave the best match, 0=0, 1=90, 2=180, 3=270

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

		identifiedMarker.push_back(Marker{ comparePattern, bestId, orientation, size });
	}

	return identifiedMarker;

}

std::vector<cv::Mat> MarkerRecognition::decode(const std::vector<cv::Mat>& normilizedMarker, const MarkerRecognition::Dictionary& dict) {

	std::vector<cv::Mat> decodeMarker;


	for (const auto& marker : normilizedMarker) {

		//Turns the makrer grey to decide blöakc and white pixels
		//Each pixel has a value of 0-255
		cv::Mat grey;
		cv::cvtColor(marker, grey, cv::COLOR_BGR2GRAY);

		//Creates a matris. CV_8UC1 means 8 bit unsinged ints and C1 = scale
		cv::Mat matris(dict.markerRows, dict.markerCols, CV_8UC1);

		//Calculate pixelsize, +2 for border
		double cellsize = 200 / (dict.markerRows + 2);

		//Divides the aröker into cells and calucalte the midpoint of each cell insted of all the pixels
		for (int i = 0; i < dict.markerRows; i++) {
			for (int j = 0; j < dict.markerCols; j++) {

				//Calculates the midpoint of each cell 
				auto x = (j + 1) * cellsize + (cellsize / 2);
				auto y = (i + 1) * cellsize + (cellsize / 2);
				
				//Uchar is unsigned, cant be 0. Can be represented as 0-255
				uchar pixelValue = grey.at<uchar>(y, x); //Calculate pixxelvalue for each pixel and using uchar as the type

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



//Replace matchOrientation and matchSize with correct datatype if needed...
//Calculate and return homography matrix, pass corners of detected marker and orientation and size of the matched marker
//cv::Mat computeMarkerHomography(
//	const cv::Mat& inputImage, const std::vector<cv::Point2f>& refinedCorners, const Marker& markerMatch,
//	cv::Mat& cameraIntrinsic, cv::Mat& cameraView) {
//	
//	//markerMatch.orientation
//	//markerMatch.size
//
//	//May need to use different function.
//	vector<Point2f> corners1, corners2;
//	
//    bool found1 = cv::findChessboardCorners(cameraView, markerMatch.size, corners1);
//    bool found2 = cv::findChessboardCorners(markerMatch.pattern, markerMatch.size, corners2);
// 
//    if (!found1 || !found2)
//    {
//        cout << "Error, cannot find the chessboard corners in both images." << endl;
//        return;
//    }
// 
//    std::vector<cv::Point3f> objectPoints;
//    cv::calcChessboardCorners(patternSize, squareSize, objectPoints);//Unknown function, likely not built into openCV.
// 
//    cv::Mat distCoeffs;
// 
//    cv::Mat rvec1, tvec1;
//    cv::solvePnP(objectPoints, corners1, cameraIntrinsic, distCoeffs, rvec1, tvec1);
//    cv::Mat rvec2, tvec2;
//    cv::solvePnP(objectPoints, corners2, cameraIntrinsic, distCoeffs, rvec2, tvec2);
//
//
//	cv::Mat R_1to2 = rvec1 * rvec1.t();
//    tvec_1to2 = rvec2 * (-rvec2.t() * tvec1) + tvec2;
//
//	cv::Mat normal = (Mat_<double>(3,1) << 0, 0, 1);
//    cv::Mat normal1 = rvec1 * normal;
//
//	cv::Mat origin(3, 1, CV_64F, Scalar(0));
//    cv::Mat origin1 = R1*origin + tvec1;
//    double d_inv1 = 1.0 / normal1.dot(origin1);
//
//	cv::Mat homography = R_1to2 + d_inv1 * tvec_1to2*normal.t();
//    return homography;
//}
//
//
//cv::Mat recoverCameraTransformation(const cv::Mat& homography, const cv::Mat& cameraIntrinsics) {
//	
//	
//}
//
//cv::Mat optimiseTransformation(const cv::Mat& initialPose, cv::Mat& refinedCorners, cv::Mat& cameraIntrinsics) {
//
//
//}

	/*cv::Mat outputImage = inputImage.clone();
	cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);*/


