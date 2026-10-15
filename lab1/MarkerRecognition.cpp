
#include "MarkerRecognition.h"





	
cv::Mat MarkerRecognition::markerRecognition(const cv::Mat& inputImage, float markerLegnth) {

	
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
	const std::vector<cv::Point3f> objectPoints = {

		{-s / 2,  s / 2, 0}, //Övre vänster
		{ s / 2,  s / 2, 0}, //Övre höger
		{ s / 2, -s / 2, 0}, //nedre höger
		{-s / 2, -s / 2, 0} //nedre vänster
	};

	double w = output.cols; //boldens bredd
	double h = output.rows; //bildens höjd
	double f = w; //gissning av bränndvidden 

	// 3X3 matrix from the image
	cv::Mat cameraMatrix = (cv::Mat_<double>(3, 3) <<
		f, 0, w / 2.0,
		0, f, h / 2.0,
		0, 0, 1);

	//Creates a mat matrix with zeros, 5 rows and 1 cols (double)
	
	cv::Mat distCoeffs = cv::Mat::zeros(5, 1, CV_64F);

	 detector.detectMarkers(output, markerCorners, markerIds, rejectedCandidates);
	
	 

	//Om det finns id:s, rita ut hörnen, ids samt färg
	if (!markerIds.empty()) {

		
		cv::aruco::drawDetectedMarkers(
			output,
			markerCorners,
			markerIds,
			cv::Scalar(0, 255, 0)
		);
		//Ritar ut rejected
		cv::aruco::drawDetectedMarkers(
			output,
			rejectedCandidates,
			cv::noArray(),
			cv::Scalar(0, 0, 255)
		);

		
		//Går igenom alla markers
		for (size_t i = 0; i < markerCorners.size(); i++)
		{
			cv::Vec3d mittpunkt, rotation; //
			//Beräknar mittpunkt samt rotation
			cv::solvePnP(objectPoints, markerCorners[i], cameraMatrix,distCoeffs, rotation, mittpunkt, false, cv::SOLVEPNP_IPPE_SQUARE);
			cv::drawFrameAxes(output, cameraMatrix, distCoeffs, rotation, mittpunkt, s * 0.5f);

		}
		
	}
	
	
	

	return output;
	

}


	


	/*cv::Mat outputImage = inputImage.clone();
	cv::aruco::drawDetectedMarkers(outputImage, markerCorners, markerIds);*/


