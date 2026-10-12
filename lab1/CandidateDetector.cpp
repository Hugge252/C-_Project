#include "CandidateDetector.hpp"
#include <opencv2/geometry.hpp>

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

    // Check each detected contour
    for (const auto& contour : contours)
    {
        double contourArea = cv::contourArea(contour);

        if (contourArea > minContourArea)
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
                        cv::circle(contourImage, corner, cornerRadius,
                        cv::Scalar(blueValue, greenValue, redValue),
                        cv::FILLED);
                    }
                
                 }

            }
            else
            {
                std::vector<std::vector<cv::Point>> rejectedContour = {contour};

                cv::drawContours(contourImage, rejectedContour, -1,
                    cv::Scalar(rejectedBlueValue, rejectedGreenValue, rejectedRedValue)
                , contourThickness);
            }
        }
    }

    return contourImage;
}