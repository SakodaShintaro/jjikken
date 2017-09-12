#include"opencv2/opencv.hpp"
#include<vector>

class Camera{
public:
    Camera();
    bool detectHumanFace();
private:
    cv::VideoCapture cap_;
};

Camera::Camera() {
    cap_ = cv::VideoCapture(0);

    if (!cap_.isOpened()) {
        return;    // キャプチャのエラー処理
    }

    cap_.set( CV_CAP_PROP_FRAME_WIDTH,  160 );
    cap_.set( CV_CAP_PROP_FRAME_HEIGHT, 120 );
}

bool Camera::detectHumanFace() {
    cv::Mat im, gray;                    // 変数宣言

    // カスケード分類器の取得
    cv::CascadeClassifier cascade;

    if (!cascade.load("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml")) {
        return false;
    }

    std::vector<cv::Rect> faces;

    cap_ >> im;                            // カメラ映像の取得

    cv::cvtColor(im, gray, CV_RGB2GRAY);    // グレースケール変換

    // カスケード分類器で顔の探索
    cascade.detectMultiScale(gray, faces, 1.2, 2, CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));

    // 顔領域を矩形で囲む
    //std::vector<cv::Rect>::const_iterator r = faces.begin( );
    //for ( ; r != faces.end( ); ++r ) {
    //    cv::rectangle(im, cv::Point( r->x, r->y ), cv::Point(r->x + r->width, r->y + r->height), cv::Scalar(20, 20, 200), 3, CV_AA);
    //}

    //cv::imshow("Camera", im);                // 映像の表示

    return !faces.empty();
}
