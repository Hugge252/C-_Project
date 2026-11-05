#include "CandidateDetector.hpp"
#include <opencv2/imgproc.hpp>
//#include <opencv2/geometry.hpp>

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


    //Stores all contours found in the binary image 
    std::vector<std::vector<cv::Point>> contours;

    // Convert frame to grayscale and then to a binary image
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    //Sudda bilden lätt (5x5) för att minska brus så att kanterna blir jämnare.
    cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);

    //Gör om till svartvit bild, pixlar över 128 blir 255 och övriga 0.
    cv::threshold(gray, binary, thresholdValue, maxValue, cv::THRESH_BINARY);

    // Hitta konturer i den svartvita bilden.
    // RETR_TREE hittar även konturer inuti andra konturer (nästlade)
    //CHAIN_APPROX_SIMPLE sparar bara ändpunkterna på raka linjer vilket sparar minne.
    cv::findContours(binary, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    //Kopia av orginalbilden som vi ritar på så origanlet förblir orört
    cv::Mat contourImage = frame.clone();

    // Check each detected contour
    for (const auto& contour : contours)
    {
        double contourArea = cv::contourArea(contour);
        
        //Ignorera små konturer(troligen brus)
        if (contourArea > minContourArea)
        {
            std::vector<cv::Point> approximate;

            double perimeter = cv::arcLength(contour, true);


            double epsilon = epsilonFactor * perimeter;

            
            //Förenkla konturen till en polygon med så få hörn som möjligt
            cv::approxPolyDP(contour, approximate, epsilon, true);

            //Fyrkant har exakt 4 hörn och måste vara konvex
            if (approximate.size() == 4 && cv::isContourConvex(approximate))
            {
                cv::Rect boundingBox = cv::boundingRect(approximate);

                double aspectRatio =
                    static_cast<double>(boundingBox.width) / boundingBox.height;

                if (aspectRatio >= minAspectRatio && aspectRatio <= maxAspectRatio)
                {
                    std::vector<std::vector<cv::Point>> filteredContour = {contour};
                    
                    //Ritar godkända konturen i grönt
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
            {   //Ritas röd eftersom den inte var konvex fyrhörning.
                std::vector<std::vector<cv::Point>> rejectedContour = {contour};

                cv::drawContours(contourImage, rejectedContour, -1,
                    cv::Scalar(rejectedBlueValue, rejectedGreenValue, rejectedRedValue)
                , contourThickness);
            }
        }
    }

    return contourImage;
}