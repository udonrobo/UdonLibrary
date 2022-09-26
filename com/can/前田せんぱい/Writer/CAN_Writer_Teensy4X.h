#pragma once
#include<arduino.h>

#ifndef CAN_HEAD
#define CAN_HEAD
#include<FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> Can1;
void CAN_begin() {
  Can1.begin();
  Can1.setClock(CLK_60MHz);//コントローラーに入力するクロック周波数の設定
  Can1.setBaudRate(500000);//通信速度の設定
  Can1.enableFIFO();
  Can1.enableFIFOInterrupt();
}
#endif

class CANWriter {
  private:
    const uint8_t My_id;
    const uint8_t Slave_id;
    const int16_t Size[3];  // 全体サイズ 横　縦
    uint8_t* data;
    CAN_message_t msg;
  public:
    CANWriter(uint8_t MyAddress, uint8_t SlaveAddress, int16_t DataSize)
    : My_id(MyAddress)
    , Slave_id(SlaveAddress)
    , Size{DataSize, (int16_t)constrain(DataSize, 0, 6) ,(int16_t)round((double)DataSize / 6 + (double)1 / 3)} {
      data = new uint8_t[Size[0]];
      msg.id = My_id;
      for (int i = 0; i < Size[0]; i++)
        data[i] = 0;
    }
    void Update() {
      for (int i = 0; i < Size[2]; i++) {
        msg.buf[0] = Slave_id;
        msg.buf[1] = i;
        for (int j = 0; j < Size[1]; j++) {
          msg.buf[j + 2] = data[i * 6 + j];
        }
        while (!Can1.write(msg));
      }
    }



    
    void SetSingleData(int array_num, uint8_t single_data) {
      data[array_num - 1] = single_data;
    }
    void SetBitData(int array_num, uint8_t bit_num, bool bit_data) {
      bitWrite(data[array_num - 1], bit_num, bit_data);
    }

    void SetMotorData(uint8_t port_num, int16_t pwm) {
      SetBitData(Size[0], port_num - 1, ((pwm < 0) ? 0 : 1)); //モーターは五つのデータ(pwm×4、方向1つ)をやりくりする。この行では５番目のデータに方向(0か1)を代入　port_num1～4　size5 方向のビットは最後に送られる(一番値が大きい)
      SetSingleData(port_num, abs(pwm)); //port_numの数字が小さい順にpwmが送られる
    }
    void reset() {
      for (int i = 0; i < Size[0]; i++)
        data[i] = 0;
    }
    void m_reset() {
      for (int i = 0; i < Size[0] - 1; i++)
        data[i] = 0;
    }
    void show() {
      //Serial.print(Size[0]); Serial.print("\t");
      //Serial.print(Size[1]); Serial.print("\t");
      //Serial.print(Size[2]); Serial.print("\t");
      for (int i = 0; i < Size[0]; i++) {
        Serial.print(data[i]);
        Serial.print("\t");
      }
    }

};
