#pragma once

#include <FlexCAN_T4.h>

class CanBase {
  protected:
    static uint8_t MASTER_ID;  // マスターID(255固定)
    static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> can;  // CANライブラリインスタンス

    static void begin() {
      static bool isBegined;
      if (isBegined)return;
      
      // 通信が開始されていない場合通信開始
      can.begin();
      can.setClock(CLK_60MHz);
      can.setBaudRate(1000000);  // 500000 だと通信できない場合があった
      can.enableFIFO();
      can.enableFIFOInterrupt();
      
      isBegined = true;
    }

};

// static メンバ初期化
uint8_t CanBase::MASTER_ID = 255;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> CanBase::can;
