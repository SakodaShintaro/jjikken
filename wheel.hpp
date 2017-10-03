#include <wiringPi.h>
#include <wiringPiSPI.h>

//ホイールに繋がっているモーターを制御するクラス
//RightとLeftの二つインスタンスを生成することを想定
//Motorクラスを作って、直接的なやりとりはモーターに任せるのが適切に思える……
class Wheel {
public:
    Wheel(bool isRightWheel);

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
    static int unit_speed;

    //信号を送る
    void sendRunSignal() const;
};

void Wheel::sendRunSignal() const {
    // 方向検出
    unsigned short dir = (speed_ < 0 ? 0x50 : 0x51);

    //スピードを絶対値に直す
    unsigned long spd = std::abs(speed_);

    // 送信バイトデータ生成。
    unsigned char spd_h, spd_m, spd_l;
    spd_h = (unsigned char)((0x0F0000 & spd) >> 16);
    spd_m = (unsigned char)((0x00FF00 & spd) >> 8);
    spd_l = (unsigned char)(0x00FF & spd);

    // コマンド（レジスタアドレス）送信。
    L6470_write(dir);
    // データ送信。
    L6470_write(spd_h);
    L6470_write(spd_m);
    L6470_write(spd_l);
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
    channel =(isRightWheel ? 1 : 0);

    // MAX_SPEED設定。
    /// レジスタアドレス。
    wiringPiSPIDataRW(channel_, 0x07, 1);
    // 最大回転スピード値(10bit) 初期値は 0x41
    wiringPiSPIDataRW(channel_, 0x00, 1);
    wiringPiSPIDataRW(channel_, 0x25  1);

    // KVAL_HOLD設定。
    /// レジスタアドレス。
    wiringPiSPIDataRW(channel_, 0x09, 1);
    // モータ停止中の電圧設定(8bit)
    wiringPiSPIDataRW(channel_, 0xFF, 1);

    // KVAL_RUN設定。
    /// レジスタアドレス。
    wiringPiSPIDataRW(channel_, 0x0A, 1);
    // モータ定速回転中の電圧設定(8bit)
    wiringPiSPIDataRW(channel_, 0xFF, 1);

    // KVAL_ACC設定。
    /// レジスタアドレス。
    wiringPiSPIDataRW(channel_, 0x0B, 1);
    // モータ加速中の電圧設定(8bit)
    wiringPiSPIDataRW(channel_, 0xFF, 1);

    // KVAL_DEC設定。
    /// レジスタアドレス。
    wiringPiSPIDataRW(channel_, 0x0C, 1);
    // モータ減速中の電圧設定(8bit) 初期値は 0x8A
    wiringPiSPIDataRW(channel_, 0x40, 1);

    // OCD_TH設定。
    /// レジスタアドレス。
    wiringPiSPIDataRW(channel_, 0x13, 1);
    // オーバーカレントスレッショルド設定(4bit)
    wiringPiSPIDataRW(channel_, 0x0F, 1);

    // STALL_TH設定。
    /// レジスタアドレス。
    wiringPiSPIDataRW(channel_, 0x14, 1);
    // ストール電流スレッショルド設定(4bit)
    wiringPiSPIDataRW(channel_, 0x7F, 1);

    //start slopeデフォルト
    /// レジスタアドレス。
    wiringPiSPIDataRW(channel_, 0x0e, 1);
    wiringPiSPIDataRW(channel_, 0x00, 1);

    //デセラレーション設定
    /// レジスタアドレス。
    wiringPiSPIDataRW(channel_, 0x10, 1);
    wiringPiSPIDataRW(channel_, 0x29, 1);

    speed_ = 0:
}
