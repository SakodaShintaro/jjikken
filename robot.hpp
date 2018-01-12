#ifndef ROBOT_HPP
#define ROBOT_HPP

//ロボットの動作を管理するクラス
#include"wheel.hpp"
#include"camera.hpp"
#include"distance_sensor.hpp"
#include"arm.hpp"
#include<cassert>
#include<cstdio>
#include<vector>
#include<thread>
#include<fcntl.h>
#include<stdlib.h> 
#include"/usr/include/linux/i2c-dev.h"

class Move;

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

    //方向転換
    void stopAndTurn(Direction direction);
    void stopAndTurn(Direction direction, double p);
    void turn(int speed);
    
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

    //物体に近づく(超音波センサを使う)
    void approachObject();

    //線分認識した結果を表示する(デバッグ用)
    void showNowView() {
        camera_.show();
    }

    //将棋指す(これがアプリケーション)
    void playShogi();

    //物を掴む.離す
    void catchObject();
    void prepareForCatch();
    void catchAndUp();
    void releaseObject(bool twist);

    //床のマスに沿って進む
    void turn90(Direction direction);
    void goSquare(int n);
    void backSquare(int n);

    //駒を画像認識して掴める位置に移動する(前後方向のみ)
    void goGoodPosition();

    //将棋盤の内部状態を一手動かす
    void doMove(const Move& move);

private:
    //(1)出力類
    //(1)-a 左右のホイール
    Wheel right_wheel_, left_wheel_;

    //止まっている状態から発進するときの初期スピード
    static const int default_speed = 10000;

    //(1)-b アーム
    Arm arm_;

    //(1)-c マイクも必要か

    //(2)入力類
    //(2)-a カメラ
    Camera camera_;

    //(2)-b マイク
    ///カメラについてる
    ///しかし間に合わなかった……

    //(2)-c センサ類
    //超音波距離センサ
    DistanceSensor ds_;

    //今何かアプリケーションを実行中だとして、それを中断するシグナル
    bool stop_signal_;
};

#endif
