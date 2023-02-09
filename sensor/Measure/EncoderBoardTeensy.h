#pragma once
#include <Wire.h>

class EncoderBoardTeensy {

  protected:
    int ADDR; //I2Cアドレス
    long offsetCount[6], nowCount[6];

  public:

    EncoderBoardTeensy(int address) : ADDR(address) {
      for (int i = 0; i < 6; i++) {
        nowCount[i] = 0;
        offsetCount[i] = 0;
      }
    }

    /*
      デストラクタ
    */
    virtual ~EncoderBoardTeensy() {}

    /*
         Teensyから全てのエンコーダの値を受信
    */
    void update() {
      long buf[6][4];
      Wire.requestFrom(ADDR, 24); //データ要求
      while (Wire.available() >= 24) {
        for (int i = 0; i < 6; i++) {
          for (int j = 0; j < 4; j++)
            buf[i][j] = Wire.read();
        }
      }

      //ビットシフトして4byteに復元
      for (int i = 0; i < 6; i++)
        nowCount[i] = (buf[i][0] << 24) | (buf[i][1] << 16) | (buf[i][2] << 8) | buf[i][3];
    }

    /*
        エンコーダ基板の初期化
        setup関数で必ず呼び出し
    */
    void init() {
      //updateを二回やらないと値を取りこぼす
      for (int i = 0; i < 2; i++)
        update();

      resetCount();
    }

    /*
      　エンコーダの回転数を取得
        @param port 基板のポート番号
    */
    long getCount(int port) {
      return nowCount[port - 1] - offsetCount[port - 1];
    }

    /*
      現在のエンコーダの値を設定した値に無理やり変更する
      @param port 基板のポート番号
      @param val 変更したい値
    */
    void setCount(int port, long val) {
      resetCount(port);
      offsetCount[port - 1] -= val;
    }

    /*
        オーバーロード
      　エンコーダの全ポートの回転数を取得
        @param val[] 値を受け取る配列
    */
    void getCount(long val[]) {
      for (int i = 0; i < 6; i++)
        val[i] = nowCount[i] - offsetCount[i];
    }

    /*
        エンコーダの回転数の初期化
        @param port 基板のポート番号
    */
    virtual void resetCount(int port) {
      offsetCount[port - 1] = nowCount[port - 1];
    }

    /*
        オーバーロード
        エンコーダの全ポートの回転数の初期化
    */
    virtual void resetCount() {
      for (int i = 0; i < 6; i++)
        offsetCount[i] = nowCount[i];
    }
};
