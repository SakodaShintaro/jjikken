#ifndef CAMERA_HPP
#define CAMERA_HPP

#include"opencv2/opencv.hpp"
#include<vector>

class Camera{
public:
    Camera();
    bool detectHumanFace(int& distance_from_center);
    void show();
    std::vector<double> getGradient();
private:
    cv::VideoCapture cap_;
};

#endif
