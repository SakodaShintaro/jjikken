#include"camera.hpp"

Camera::Camera() {
    cap_ = cv::VideoCapture(0);

    if (!cap_.isOpened()) {
        std::cerr << "VideoCapture cannot open" << std::endl;
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
        distance_from_center = faces[0].x + faces[0].width / 2;
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
        cv::Mat gray_im, binary_im;
        cv::cvtColor(im, gray_im, CV_BGR2GRAY);
        //cv::GaussianBlur(gray_im, gray_im, cv::Size(5, 5), 2.5, 2.5);
        cv::imshow("gray_im", gray_im);
        cv::Canny(gray_im, binary_im, 30, 100, 3);
        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(binary_im, lines, 1.0, CV_PI / 180, 50, 100, 20);
        for (auto line : lines) {
            //float rho = line[0];
            //float theta = line[1];
            //double a = cv::cos(theta), b = cv::sin(theta);
            //double x0 = a * rho, y0 = b * rho;
            //cv::Point p1(cvRound(x0 + 1000 * (-b)), cvRound(y0 + 1000 * a));
            //cv::Point p2(cvRound(x0 - 1000 * (-b)), cvRound(y0 - 1000 * a));
            //cv::line(im, p1, p2, cv::Scalar(0, 0, 255), 1, 8);
            cv::line(im, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0, 0, 255), 1, 8);
        }

        cv::imshow("binary", binary_im);
        cv::imshow("movie", im);
        if (cv::waitKey(30) >= 0) {
            std::cout << "キー入力が来てない気がする" << std::endl;
            break;
        }
    }
    cv::destroyWindow("movie");
}
