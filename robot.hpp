//ロボットの動作を管理するクラス
#include"wheel.hpp"

class Robot {
public:
    Robot();
    void goForward(int speed = 1000);
    void back(int speed = 1000);
    void stop();
    void stopAndTurnRight();
    void stopAndTurnLeft();
    void speedUp();
    void speedDown();

    //たぶんループが必要
    //画像,音声,センサ入力とかからsignalが来ているか確認して、それに適した動作をホイールもしくはアームに伝える
    void loop();

private:
    //左右の両輪
    Wheel right_wheel_(true), left_wheel(false);

    //カメラ入力
    //openCVを使うはず

    //センサ類
};

void Robot::goForward(int speed) {
    right_wheel.run(speed);
    left_wheel.run(speed);
}

void Robot::back(int speed) {
    right_wheel.run(-speed);
    left_wheel.run(-speed);
}

void Robot::stop() {
    right_wheel.gradualStop();
    left_wheel.gradualStop();
}

void Robot::turnRight() {
}
