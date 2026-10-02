#include <iostream>
#include <opencv2/opencv.hpp>
int main()
{
	// githubCommitCheck
	// Open the supplied video file.
	cv::VideoCapture video("resources/camera_test.mp4");

	if (!video.isOpened()) {
		std::cerr << "Could not open the video.\n";
		return 1;
	}

	//håller på att testa QR image
	//cv::Mat testImage("resources/qrcode.png");
	/*if (testImage.empty()) {
		std::cerr << "Could not load qrcode.png\n";
		return 1;
	}*/

	cv::QRCodeDetector qrDetector;
	cv::Mat frame;
	cv::Mat frameGray;


	// Read and display one frame at a time.
	while (video.read(frame)) {

		cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);
		cv::imshow("OpenCV video test", frameGray);

		std::string data = qrDetector.detectAndDecode(frameGray);
		if (!data.empty()) {
			std::cout << "QR code hittad!!" << data << "\n";
		}
		
		// Wait briefly; Escape closes the program.
		if (cv::waitKey(20) == 27) {
			break;
		}
	}
	return 0;
}