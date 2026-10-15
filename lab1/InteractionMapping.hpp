class InteractionMapping {
    private:   
        int currentAngle = 0;//The last recorded rotation of the marker. Currently unused.

    public:
        int getRotation(const std::vector<cv::Point2f>& corners, const cv::Point2f& topLeft);
};