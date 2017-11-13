#include"opencv2/opencv.hpp"
#include<vector>

class Camera{
public:
    Camera();
    bool detectHumanFace(int& distance_from_center);
    void show();
private:
    cv::VideoCapture cap_;
};

Camera::Camera() {
    cap_ = cv::VideoCapture(0);

    if (!cap_.isOpened()) {
        return;    // キャプチャのエラー処理
    }

    cap_.set(CV_CAP_PROP_FRAME_WIDTH,  160);
    cap_.set(CV_CAP_PROP_FRAME_HEIGHT, 120);
}

bool Camera::detectHumanFace(int& distance_from_center) {
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

    if (faces.size() != 1) {
        distance_from_center = 0;
    } else {
        distance_from_center = faces[0].x;
    }

    // 顔領域を矩形で囲む
    //std::vector<cv::Rect>::const_iterator r = faces.begin( );
    //for ( ; r != faces.end( ); ++r ) {
    //    cv::rectangle(im, cv::Point( r->x, r->y ), cv::Point(r->x + r->width, r->y + r->height), cv::Scalar(20, 20, 200), 3, CV_AA);
    //}

    //cv::imshow("Camera", im);                // 映像の表示

    return !faces.empty();
}

void Camera::show() {
    cv::namedWindow("movie", cv::WINDOW_AUTOSIZE);
    while (true) {
        cv::Mat im;
        cap_ >> im;
        if (im.empty()) {
            break;
        }

        //imの加工
        cvtColor(im, im, CV_BGR2GRAY);
        threshold(im, im, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
        std::vector<std::vector<cv::Point> > contours;
        cv::findContours(im, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
        std::vector<std::vector<cv::Point> > contours_subset;
        for (int i = 0; i < contours.size(); i++) {
            double area = contourArea(contours[i]);
            //printf("%f\n", area);
            if (area > 5000 && area < 15000) {
                contours_subset.push_back(contours[i]);
            }
        }

        cv::imshow("movie", im);
        if (cv::waitKey(30) >= 0) {
            std::cout << "キー入力が来てない気がする" << std::endl;
            break;
        }
    }
    cv::destroyWindow("movie");
}
