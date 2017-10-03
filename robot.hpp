//ロボットの動作を管理するクラス
#include"wheel.hpp"

class Robot {
public:
    Robot();
    //移動類
    //方向を示す定数
    enum Direction {
        FORWARD, BACK, RIGHT, LEFT
    };
    void run(Direction direction, int speed = default_speed);
    void stop();

    //90度,180度方向転換するイメージだけど、角度はどうやって測るのかわからない
    void stopAndTurn(Direction direction);
    void curve
    void speedUp();
    void speedDown();

    //たぶんループが必要
    //画像,音声,センサ入力とかからsignalが来ているか確認して、それに適した動作をホイールもしくはアームに伝える
    void loop();

private:
    //左右の両輪
    Wheel right_wheel_(true), left_wheel(false);

    static int default_speed = 10000;

    //カメラ
    //openCVを使うはず
    //うーん、どういう設計になるのかうまく創造できてない……

    //マイク

    //センサ類
};

void Robot::run(Direction direction, int speed) {
    switch (direction) {
    case FORWARD:
        right_wheel.run(speed);
        left_wheel.run(speed);
        break;
    case BACK:
        right_wheel.run(-speed);
        left_wheel.run(-speed);
        break;
    case RIGHT:
        stopAndTurn(RIGHT);
        run(FORWARD, speed);
        break;
    case LEFT:
        stopAndTurn(LEFT);
        run(FORWARD, speed);
        break;
    default:
        //ここにはこないはず
        assert(false);
    }
}

void Robot::stop() {
    right_wheel.gradualStop();
    left_wheel.gradualStop();
}

void Robot::stopAndTurn(Direction direction) {
    assert(direction == RIGHT || direction == LEFT);
    switch (direction) {
    case RIGHT:
        right_wheel.run(-default_speed / 2);
        left_wheel.run(+default_speed / 2);
        break;
    case LEFT:
        right_wheel.run(+default_speed / 2);
        left_wheel.run(-default_speed / 2);
        break;
    }
}
