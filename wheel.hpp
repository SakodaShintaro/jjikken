//手元にはwiringPiなんてないからコメントアウトしておく
#include<cstdlib>
#include<iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>

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

void Wheel::sendRunSignal() {
    // 方向検出
    speed_ *= (channel_ == 1 ? -1 : 1);
    unsigned short dir = (speed_ < 0 ? 0x50 : 0x51);

    //スピードを絶対値に直す
    unsigned long spd = std::abs(speed_);

    // 送信バイトデータ生成。
    unsigned char spd_h, spd_m, spd_l;
    spd_h = (unsigned char)((0x0F0000 & spd) >> 16);
    spd_m = (unsigned char)((0x00FF00 & spd) >> 8);
    spd_l = (unsigned char)(0x00FF & spd);

    // コマンド（レジスタアドレス）送信。
    write(dir);
    // データ送信。
    write(spd_h);
    write(spd_m);
    write(spd_l);
}

void Wheel::run(int speed) {
    speed_ = speed;
    sendRunSignal();
}

void Wheel::speedUp() {
    speed_ += unit_speed;
    sendRunSignal();
}

void Wheel::speedDown() {
    speed_ += unit_speed;
    sendRunSignal();
}

void Wheel::emergencyStop() {
    speed_ = 0;
    sendRunSignal();
}

void Wheel::gradualStop() {
    while (speed_ > 0) {
        speed_ -= unit_speed;
        sendRunSignal();
    }
}

Wheel::Wheel(bool isRightWheel) {
    //チャンネルの設定と,モーターのもろもろの設定をやるっぽい
    channel_ =(isRightWheel ? 1 : 0);

    if (wiringPiSPISetup(channel_, 1000000) < 0){
        printf("SPI Setup failed:\n");
    }

    // MAX_SPEED設定。
    /// レジスタアドレス。
    write(0x07);
    // 最大回転スピード値(10bit) 初期値は 0x41
    write(0x00);
    write(0x25);

    // KVAL_HOLD設定。
    /// レジスタアドレス。
    write(0x09);
    // モータ停止中の電圧設定(8bit)
    write(0xFF);

    // KVAL_RUN設定。
    /// レジスタアドレス。
    write(0x0A);
    // モータ定速回転中の電圧設定(8bit)
    write(0xFF);

    // KVAL_ACC設定。
    /// レジスタアドレス。
    write(0x0B);
    // モータ加速中の電圧設定(8bit)
    write(0xFF);

    // KVAL_DEC設定。
    /// レジスタアドレス。
    write(0x0C);
    // モータ減速中の電圧設定(8bit) 初期値は 0x8A
    write(0x40);

    // OCD_TH設定。
    /// レジスタアドレス。
    write(0x13);
    // オーバーカレントスレッショルド設定(4bit)
    write(0x0F);

    // STALL_TH設定。
    /// レジスタアドレス。
    write(0x14);
    // ストール電流スレッショルド設定(4bit)
    write(0x7F);

    //start slopeデフォルト
    /// レジスタアドレス。
    write(0x0e);
    write(0x00);

    //デセラレーション設定
    /// レジスタアドレス。
    write(0x10);
    write(0x29);

    speed_ = 0;
}
