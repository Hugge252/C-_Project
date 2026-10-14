class InteractionMapping {
    private:   
        //const int inUnityScale = 1;//Figure out what this value should be. may not be needed
        
        int currentAngle = 0;//The last recorded rotation of the marker.

    public:
        int getRotation(const std::vector<cv::Point2f> corners, const cv::Point2f topLeft);
};