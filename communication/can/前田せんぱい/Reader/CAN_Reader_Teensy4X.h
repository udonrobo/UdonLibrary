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
  //下の2行がよく分からない(信号を受け取るのに必要だった)
  Can1.enableFIFO();
  Can1.enableFIFOInterrupt();
}
#endif

class CANReader {
  private:
    const uint8_t MyID;
    int16_t Size[256][3];
    uint8_t **data;
    unsigned long time[256];
    bool Reading;
    bool bufReset;
  public:
    CANReader(uint8_t MyAddress): MyID(MyAddress) {
      data = new uint8_t *[256];
    }
    void SetAddress(uint8_t SenderAddress, int DataSize) {
      data[SenderAddress] = new uint8_t[DataSize];
    }
    void Update() {
      Reading = false;
      bufReset = false;
      Can1.events();
    }
    void read(CAN_message_t &msg) {
      bufReset = true;
      if (msg.buf[0] == MyID) {
        Reading = true;
        bufReset = false;
        time[msg.id] = millis();
        for (int i = 0; i < msg.len - 2; i++) {
          if (msg.buf[1] * 6 + i < Size[msg.id][0])
            data[msg.id][msg.buf[1] * 6 + i] = msg.buf[i + 2];
        }
      }
      Update();
    }
    uint8_t GetData(int SenderAddress, int data_num) {
      return data[SenderAddress][data_num - 1];
    }
    bool GetBitData(int SenderAddress, int data_num, int bit_num) {
      return bitRead(data[SenderAddress][data_num - 1], bit_num);
    }
    void show(int Address) {
      for (int i = 0; i < Size[Address][0]; i++) {
        Serial.print(data[Address][i]);
        Serial.print("\t");
      }
    }
    unsigned long GetTime(int Address) {
      return time[Address];
    }
    bool reading() {
      return bufReset || Reading;
    }
};

extern CANReader test;
