#include "CandidateDetector.hpp"
#include <opencv2/opencv.hpp>
//#include <opencv2/geometry.hpp>
//Added include opencv2/opencv.hpp instead of opencv2/geometry.hpp. It prevented me from building,
//may be some difference in opencv versions so if this was not an issue feel free to revert. - O

//No major changes made. Mainly small visual changes and a test output for amount of contours. - O
cv::Mat CandidateDetector::detect(const cv::Mat& frame)
{
    cv::Mat gray;
    cv::Mat binary;

    // Values
    int thresholdValue = 128;
    int maxValue = 255;

    // Green color for accepted candidates
    int blueValue = 0;
    int greenValue = 255;
    int redValue = 0;

    // Red color for rejected candidates
    int rejectedBlueValue = 0;
    int rejectedGreenValue = 0;
    int rejectedRedValue = 255;

    // Values used to filter contour candidates
    double minContourArea = 1000;
    double epsilonFactor = 0.02;
    double minAspectRatio = 0.5;
    double maxAspectRatio = 2.0;

    int contourThickness = 4;
    int cornerRadius = 6;

    std::vector<std::vector<cv::Point>> contours;

    // Convert frame to grayscale and then to a binary image
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);

    cv::threshold(gray, binary, thresholdValue, maxValue, cv::THRESH_BINARY);

    // Find contours, including nested contours
    cv::findContours(binary, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat contourImage = frame.clone();

    //Variables for testing: - O
    bool isTesting = false;
    int acceptedContourCountTest = 0;
    int rejectedContourCountTest = 0;

    // Check each detected contour
    for (const auto& contour : contours)
    {
        double contourArea = cv::contourArea(contour);
        
        //Switched from > operator to >= to match how minAspectRatio is used. -O
        if (contourArea >= minContourArea)
        {
            std::vector<cv::Point> approximate;

            double perimeter = cv::arcLength(contour, true);
            double epsilon = epsilonFactor * perimeter;

            cv::approxPolyDP(contour, approximate, epsilon, true);

            if (approximate.size() == 4 && cv::isContourConvex(approximate))
            {
                cv::Rect boundingBox = cv::boundingRect(approximate);

                double aspectRatio =
                    static_cast<double>(boundingBox.width) / boundingBox.height;

                if (aspectRatio >= minAspectRatio && aspectRatio <= maxAspectRatio)
                {
                    std::vector<std::vector<cv::Point>> filteredContour = {contour};

                    cv::drawContours(contourImage, filteredContour, -1,
                        cv::Scalar(blueValue, greenValue, redValue), contourThickness);

                    
                    for(const auto& corner : approximate)
                    {
                        //Pushed in 2nd and 3rd line to make it slightly more clear that these are part of the first. - O
                        cv::circle(contourImage, corner, cornerRadius,
                            cv::Scalar(blueValue, greenValue, redValue),
                            cv::FILLED);
                    }
                    
                    acceptedContourCountTest++;
                }

            }
            else
            {
                std::vector<std::vector<cv::Point>> rejectedContour = {contour};
                //Moved a comma slightly. - O
                cv::drawContours(contourImage, rejectedContour, -1,
                    cv::Scalar(rejectedBlueValue, rejectedGreenValue, rejectedRedValue),
                    contourThickness);

                rejectedContourCountTest++;
            }
        }
    }

    //Added test output to show how many accepted and rejected contours were found by the script. - O
    //Can be compared to how many are seen on screen to determine if something is counted but not visible.
    if(isTesting == true)
    {
        std::cout << "Candidate detector found:\n" << acceptedContourCountTest
            << " accepted contours and " << rejectedContourCountTest << " rejected contours.\n";
    }

    return contourImage;
}