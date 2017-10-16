//ロボットの動作を管理するクラス
#include"wheel.hpp"
#include<cassert>
#include<cstdio>
#include<vector>
#include<thread>
#include"opencv2/opencv.hpp"

class Robot {
public:
    Robot() : right_wheel_(true), left_wheel_(false){}
    //移動類
    //方向を示す定数
    enum Direction {
        FORWARD, BACK, RIGHT, LEFT
    };

    //一番標準的な方向を指定して進む関数
    void run(Direction direction, int speed = default_speed);
    
    //止まる
    void stop();

    //特定の方向に距離を指定して進む関数が欲しくなりそうな気もする

    //90度,180度方向転換するイメージだけど、角度はどうやって測るのか
    void stopAndTurn(Direction direction);
    
    //前に進んでいる状態で呼び出す想定
    //方向転換ではなく曲がりながら進む
    void curve(Direction direction);

    //速度変更機能実は要らないんじゃないか説もある
    void speedUp();
    void speedDown();

    //たぶんループが必要
    //画像,音声,センサ入力とかを監視して、それに対して適切な動作をホイールもしくはアームに送る
    //というか、そういうアプリを呼び出すということになるのかなぁ
    //Robotクラスが膨らみそう
    //今はコマンド入力を待つだけ～
    void loop();

    //人の顔
    void traceHumanFace();
    void seeHumanFace();

private:
    //(1)出力類
    //(1)-a 左右のホイール
    Wheel right_wheel_, left_wheel_;

    //止まっている状態から発進するときの初期スピード
    static const int default_speed = 10000;

    //(1)-b アーム
    //Arm arm_;

    //(1)-c マイクも必要か


    //(2)入力類
    //(2)-a カメラ
    //openCVを使うはず
    //うーん、どういう設計になるのかうまく想像できてない……
    bool isThereFace_;

    //(2)-b マイク

    //(2)-c センサ類
    //超音波距離センサ?
};

void Robot::run(Direction direction, int speed) {
    switch (direction) {
    case FORWARD:
        std::cout << "前進" << std::endl;
        right_wheel_.run(speed);
        left_wheel_.run(speed);
        break;
    case BACK:
        std::cout << "後進" << std::endl;
        right_wheel_.run(-speed);
        left_wheel_.run(-speed);
        break;
    case RIGHT:
        //右を向いてから前進
        std::cout << "右を向いてから前進" << std::endl;
        stopAndTurn(RIGHT);
        run(FORWARD, speed);
        break;
    case LEFT:
        //左を向いてから前進
        std::cout << "左を向いてから前進" << std::endl;
        stopAndTurn(LEFT);
        run(FORWARD, speed);
        break;
    default:
        //ここには来ないはず
        assert(false);
    }
}

void Robot::stop() {
    std::cout << "停止" << std::endl;
    right_wheel_.gradualStop();
    left_wheel_.gradualStop();
}

void Robot::stopAndTurn(Direction direction) {
    switch (direction) {
    case RIGHT:
        std::cout << "右を向く" << std::endl;
        right_wheel_.run(-default_speed / 2);
        left_wheel_.run(+default_speed / 2);
        //どれくらい回ったら止まるかも判断したけど……
        break;
    case LEFT:
        std::cout << "左を向く" << std::endl;
        right_wheel_.run(+default_speed / 2);
        left_wheel_.run(-default_speed / 2);
        //どれくらい回ったら止まるかも判断したけど……
        break;
    default:
        //ここには来ないはず
        assert(false);
    }
}

void Robot::loop() {
    std::cout << "loop開始" << std::endl;
    std::cout << "コマンドを入力してください" << std::endl;
    char c;
    while (std::cin >> c) {
        switch (c) {
        case 'p':
            speedUp();
            break;
        case 'q':
            speedDown();
            break;
        case 'f':
            run(FORWARD, default_speed);
            break;
        case 'b':
            run(BACK, default_speed);
            break;
        case 'r':
            curve(RIGHT);
            break;
        case 'l':
            curve(LEFT);
            break;
        case 'R':
            stopAndTurn(RIGHT);
            break;
        case 'L':
            stopAndTurn(LEFT);
            break;
        case 's':
            stop();
            break;
        case 'a':
            traceHumanFace();
            break;
        case 'h':
            printf("p : speedUp\n");
            printf("q : speedDown\n");
            printf("f : goForward\n");
            printf("b : goBack\n");
            printf("r : curveRight\n");
            printf("l : curveLeft\n");
            printf("R : turnRight\n");
            printf("L : turnLeft\n");
            printf("s : stop\n");
            printf("a : traceHumanFace\n");
            break;
        case 'x':
            return;
        default:
            std::cout << "不正な入力" << std::endl;
        }
    }
}

void Robot::speedUp() {
    right_wheel_.speedUp();
    left_wheel_.speedUp();
}

void Robot::speedDown() {
    right_wheel_.speedDown();
    left_wheel_.speedDown();
}

void Robot::curve(Direction direction) {
    switch (direction) {
    case RIGHT:
        std::cout << "右に曲がる" << std::endl;
        left_wheel_.speedUp();
        break;
    case LEFT:
        std::cout << "左に曲がる" << std::endl;
        right_wheel_.speedUp();
        break;
    default:
        //ここには来ないはず
        assert(false);
    }
}

void Robot::traceHumanFace() {
    std::thread see(&Robot::seeHumanFace, this);
    while (true) {
        if (isThereFace_) {
            run(FORWARD);
        } else {
            stop();
        }
    }
}

void Robot::seeHumanFace() {
    cv::Mat im, gray;                    // 変数宣言

    // カスケード分類器の取得
    cv::CascadeClassifier cascade;

    if (!cascade.load("/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml")) {
        return;
    }

    std::vector<cv::Rect> faces;
    cv::VideoCapture cap(0);            // カメラのキャプチャ

    if (!cap.isOpened()) {
        return;    // キャプチャのエラー処理
    }

    cap.set( CV_CAP_PROP_FRAME_WIDTH,  160 );
    cap.set( CV_CAP_PROP_FRAME_HEIGHT, 120 );

    while (true) {
        cap >> im;                            // カメラ映像の取得

        cv::cvtColor(im, gray, CV_RGB2GRAY);    // グレースケール変換

        // カスケード分類器で顔の探索
        cascade.detectMultiScale(gray, faces, 1.2, 2, CV_HAAR_SCALE_IMAGE, cv::Size(20, 20));

        // 顔領域を矩形で囲む
        if (!faces.empty()) {
            isThereFace_ = true;
        }
        //vector<Rect>::const_iterator r = faces.begin( );
        //for ( ; r != faces.end( ); ++r ) {
        //  rectangle( im, Point( r->x, r->y ), Point(r->x + r->width, r->y + r->height), Scalar(20, 20, 200), 3, CV_AA);
        //}

        //imshow( "Camera", im );                // 映像の表示
        if (cv::waitKey( 30 ) >= 0)
            break;                               // キー入力があれば終了
    }
}
