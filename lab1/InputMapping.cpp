#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>

//Get input in form of marker rotation/corner positions. Convert into an output of a rotation and coordinates usable in Unity.
//Decide on what size the marker is meant to be to allow camera movement around marker while the object in Unity seems to keep its position.
class InputMap {
    public:
        std::vector<cv::Point2f> getPositionsForUnity(const std::vector<cv::Point2f>* corners){
            std::vector<cv::Point2f> markerPositions;
            
            //Loop through values and adjust each one.
            for (size_t i = 0; i < positions.size(); i++)
            {
                markerPositions.push_back(inUnityScale * positions[i]);//Calculate new values for each position.
            }

            return markerPositions;
        }

        float getRotationsForUnity(const std::vector<cv::Point2f>* corners){

            //Find current rotation of marker using corners and return value which can be used for an axis in unity.
            return 1;
        }

    private:
        const int inUnityScale = 1;//Figure out what this value should be.


};