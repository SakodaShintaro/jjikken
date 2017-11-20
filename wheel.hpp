#ifndef WHEEL_HPP
#define WHEEL_HPP

//手元にはwiringPiなんてないからコメントアウトしておく
#include<cstdlib>
#include<iostream>
#include<wiringPi.h>
#include<wiringPiSPI.h>

//ホイールを制御するクラス
//RightとLeftの二つインスタンスを生成することを想定
//Motorクラスを作って、モーターの直接的な操作はMotorクラスに任せるのが適切に思える……
class Wheel {
public:
    //bool型入れてるのちょっと美しくないですかね
    Wheel(bool isRightWheel);

    //名前が変な気がする(wheel自体は走っていることを意識しない、回るだけ)
    void run(int speed);

    void speedUp();
    void speedDown();

    //緊急停止
    void emergencyStop();

    //徐々に緩めて止める:通常の停止はこっちを使うのかな
    //そういう配慮が必要なのかわからない
    void gradualStop();

private:
    //繋がっているチャンネル
    int channel_;

    //現在の回転速度
    int speed_;

    //速度を変化させる際の単位量
    static const int unit_speed = 10000;

    //信号を送る
    void sendRunSignal();

    void write(unsigned char data) {
        wiringPiSPIDataRW(channel_, &data, 1);
    }
};

#endif
