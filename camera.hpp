#ifndef CAMERA_HPP
#define CAMERA_HPP

#include<opencv2/opencv.hpp>
#include<vector>

class Camera{
public:
    Camera();
    bool detectHumanFace(int& distance_from_center);
    bool detectPiece(cv::Point& p);
    void show();
    void capture();
    std::vector<double> getGradient();
    double getVerticalLineX();
    std::vector<double> getHorizontalLineY();

    bool detectSomething();
private:
    cv::VideoCapture cap_;

};

#endif
