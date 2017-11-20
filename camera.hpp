#ifndef CAMERA_HPP
#define CAMERA_HPP

#include"opencv2/opencv.hpp"

class Camera{
public:
    Camera();
    bool detectHumanFace(int& distance_from_center);
    void show();
private:
    cv::VideoCapture cap_;
};

#endif
