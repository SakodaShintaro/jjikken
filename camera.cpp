#include"camera.hpp"

Camera::Camera() {
    cap_ = cv::VideoCapture(1);

    if (!cap_.isOpened()) {
        std::cerr << "VideoCapture cannot open" << std::endl;
        return;    // キャプチャのエラー処理
    }

    //cap_.set(CV_CAP_PROP_FRAME_WIDTH,  160);
    //cap_.set(CV_CAP_PROP_FRAME_HEIGHT, 120);
    //もっと解像度上げていいだろう
    cap_.set(CV_CAP_PROP_FRAME_WIDTH,  640);
    cap_.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
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
    std::vector<cv::Rect>::const_iterator r = faces.begin( );
    for ( ; r != faces.end( ); ++r ) {
        cv::rectangle(im, cv::Point( r->x, r->y ), cv::Point(r->x + r->width, r->y + r->height), cv::Scalar(20, 20, 200), 3, CV_AA);
    }

    cv::imshow("Camera", im);                // 映像の表示
    cv::waitKey(0);

    return !faces.empty();
}

bool Camera::detectPiece(cv::Point& p) {
    cv::Mat im, gray;                    // 変数宣言

    // カスケード分類器の取得
    static cv::CascadeClassifier cascade;

    if (cascade.empty()) {
        if (!cascade.load("../work/cascade/cas14/cascade.xml")) {
            fprintf(stderr, "casacde load error\n");
            return false;
        }
    }

    std::vector<cv::Rect> pieces;

    cap_ >> im;                            // カメラ映像の取得

    cv::cvtColor(im, gray, CV_RGB2GRAY);    // グレースケール変換

    //カスケード分類器で探索
    cascade.detectMultiScale(gray, pieces, 1.05, 2, CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

    if (pieces.size() != 1) {
        p.x = p.y = -1;
        return false;
    } else {
        for (auto r = pieces.begin() ; r != pieces.end( ); ++r ) {
            cv::rectangle(im, cv::Point( r->x, r->y ), cv::Point(r->x + r->width, r->y + r->height), cv::Scalar(20, 20, 200), 3, CV_AA);
        }

        p.x = pieces[0].x + pieces[0].width / 2;
        p.y = pieces[0].y + pieces[0].height / 2;
        return true;
    }
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
        cv::Canny(gray_im, binary_im, 45, 100, 3);
        std::vector<cv::Vec4i> lines;
        cv::HoughLinesP(binary_im, lines, 1.0, CV_PI / 180, 50, 360, 60);
        for (auto line : lines) {
            cv::line(im, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0, 0, 255), 1, 8);
        }

        cv::imshow("binary", binary_im);
        cv::imshow("movie", im);
        if (cv::waitKey(30) >= 0) {
            std::cout << "終了" << std::endl;
            break;
        }
    }
    cv::destroyWindow("movie");
}

std::vector<double> Camera::getGradient() {
    std::vector<double> result;
    cv::Mat im;
    cap_ >> im;

    if (im.empty()) {
        std::cerr << "画像の取得に失敗" << std::endl;
        return result;
    }

    //imの加工
    cv::Mat gray_im, binary_im;
    cv::cvtColor(im, gray_im, CV_BGR2GRAY);
    cv::Canny(gray_im, binary_im, 90, 200, 3);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(binary_im, lines, 1.0, CV_PI / 180, 50, 360, 60);
    for (auto line : lines) {
        double grad = (line[0] == line[2] ? INT_MAX : (double)(line[1] - line[3]) / (line[0] - line[2]));
        result.push_back(grad);
    }

    return result;
}

double Camera::getVerticalLineX() {
    cv::Mat im;
    cap_ >> im;

    if (im.empty()) {
        std::cerr << "画像の取得に失敗" << std::endl;
        return -1;
    }

    //imの加工
    cv::Mat gray_im, binary_im;
    cv::cvtColor(im, gray_im, CV_BGR2GRAY);
    cv::Canny(gray_im, binary_im, 45, 100, 3);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(binary_im, lines, 1.0, CV_PI / 180, 50, 90, 15);
    double sum = 0;
    int num = 0;
    for (auto line : lines) {
        if (line[0] == line[2]) {
            return line[0];
        }
        if (line[2] - 5 <= line[0] && line[0] <= line[2] + 5) {
            //line[2] == line[0] +- 5 ということ
            sum += ((double)line[2] + line[0]) / 2;
            num++;
        }
    }

    if (num == 0) {
        return -1.0;
    } else {
        return sum / num;
    }
}

std::vector<double> Camera::getHorizontalLineY() {
    std::vector<double> result;
    cv::Mat im;
    cap_ >> im;

    if (im.empty()) {
        std::cerr << "画像の取得に失敗" << std::endl;
        return result;
    }

    //imの加工
    cv::Mat gray_im, binary_im;
    cv::cvtColor(im, gray_im, CV_BGR2GRAY);
    cv::Canny(gray_im, binary_im, 45, 100, 3);
    std::vector<cv::Vec4i> lines;
    cv::HoughLinesP(binary_im, lines, 1.0, CV_PI / 180, 50, 90, 15);
    for (auto line : lines) {
        double grad = (line[0] == line[2] ? INT_MAX : (double)(line[1] - line[3]) / (line[0] - line[2]));
        if (std::abs(grad) < 2) {
            result.push_back(((double)line[1] + line[3]) / 2);
        }
    }

    return result;
}


void Camera::capture() {
    cv::Mat image;
    cap_ >> image;
    if (image.empty()) {
        std::cerr << "画像の取得に失敗" << std::endl;
        return;
    }
    static unsigned int i = 0;
    cv::imwrite("./neg/bg " + std::to_string(i++) + ".jpg", image);
}


bool Camera::detectSomething()
{
    cv::Mat tmp;
    cap_ >> tmp;
    int size = tmp.cols * tmp.rows;
    //while (true) {
    cv::Mat image, medimg, grayimg, binimg;
    cap_ >> image;
    cv::medianBlur(image, medimg, 11);
    cv::cvtColor(medimg, grayimg, CV_RGB2HSV);

    std::vector<cv::Mat> planes;
    cv::split(grayimg, planes);
    //cvtColor(medimg, grayimg, CV_BGR2GRAY);

    cv::threshold(planes[1], binimg, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);
    std::vector<std::vector <cv::Point> > contours;
    cv::findContours(binimg, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    for (auto contour = contours.begin();contour != contours.end(); contour++) {
        cv::polylines(image, *contour, true, cv::Scalar(0, 255, 0), 2);
    }

    for (auto contour = contours.begin();contour != contours.end(); contour++) {
        std::vector<cv::Point> approx;
        cv::approxPolyDP(cv::Mat(*contour), approx, 0.01 * cv::arcLength(*contour, true), true);
        double area = cv::contourArea(approx);
        //if (isContourConvex(approx) && area > size/100)
        if (area > size / 50) {
            cv::polylines(image, approx, true, cv::Scalar(255, 0, 0), 2);
            return true;
        }
    }

    cv::imshow(" ", image);
    cv::waitKey(0);
    //}
    return false;
}
