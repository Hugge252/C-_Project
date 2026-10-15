#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>
#include "InteractionMapping.hpp"
    

    //Find current rotation of marker using corners and return rotation in degrees (0-360):
    int InteractionMapping::getRotation(const std::vector<cv::Point2f>& corners, const cv::Point2f& topLeft){
    if(corners.size() > 4){
        return currentAngle;
    }
  
    int topLeftIndex = -1;//Initialized at -1 to be able to be checked for if it has been changed.
    for(size_t i = 0; i < corners.size(); i++){
        if(corners[i] == topLeft){
            topLeftIndex = i;
            break;
        }
    }
    
    if(topLeftIndex == -1){
        return currentAngle;
    }            
    
    //Reorder corners to start on the top left corner. (Need to double check order).
    std::vector<cv::Point2f> reorderedCorners;
    reorderedCorners.push_back(corners[topLeftIndex]);
    int j = topLeftIndex + 1;
    while (reorderedCorners.size() <= 4){
        reorderedCorners.push_back(corners[j % 4]);
        j++;
    }
            
    //Calculate rotation from corners:
    cv::Point2f midpointA, midpointB, vectorAToB;
    //Find midpoint of index 0 and 1 (top left and top right)
    midpointA = (reorderedCorners[0]+reorderedCorners[1])/2;
    //Find midpoint of index 2 and 3 (bottom left and bottom right (may be a different order but does not matter for this calculation))
    midpointB = (reorderedCorners[2]+reorderedCorners[3])/2;
    //Create 'vector' from midpoints
    vectorAToB = midpointA - midpointB;
    //Calculate atan2 of vector, gives rotation (in radians between -pi and pi)
    float angle = std::atan2f(vectorAToB.y, vectorAToB.x);
    
    //Convert rotation to degrees between 0 and 360:
    //multiply by 180/pi (approximation) to convert to degrees
    angle *= 57.296;
    //adjust angle to where 0 degrees is 'up' and rotation goes clockwise
    angle = 90 - angle;
    //adjust angle to stay within the range 0-360, will always round down which decreases accuracy
    angle = (int)angle % 360;
          
    currentAngle = angle;
    return angle;
};
    