#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/types.hpp>

class InputMap {
    public:

    private:
        std::vector<std::vector<cv::Point2f>> markerPositions, prevPositions;
};