#pragma once

#include <FlexCAN_T4.h>

class CanSlaveWriter {

    static uint8_t MASTER_ID;
    static FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> can;

    uint8_t  myID;       // 自身のアドレス
    uint8_t  writeSize;  // 送信サイズ
    uint8_t* buf;        // 送信バッファ

  public:

    CanSlaveWriter(uint8_t myID, uint8_t writeSize)
      : myID(myID)
      , writeSize(writeSize)
      , buf(new uint8_t[writeSize])
    {
      memset(buf, 0, writeSize);
      can.begin();
      can.setClock(CLK_60MHz);  //コントローラーに入力するクロック周波数の設定
      can.setBaudRate(500000);  //通信速度の設定
      can.enableFIFO();         // hogehoge
      can.enableFIFOInterrupt();
    }

    /** 必須
       @brief バッファを送信
    */
    void update() {
      const uint8_t paketCount = ceil(writeSize / 6.);  // 送信回数
      for (uint8_t i = 0; i < paketCount; i++) {
        CAN_message_t msg;
        msg.id     = myID;
        msg.buf[0] = MASTER_ID;
        msg.buf[1] = i;

        for (uint8_t j = 0; j < 6; j++) {
          const uint8_t bufferIndex = j + i * 6;
          if (bufferIndex < writeSize)
            msg.buf[j + 2] = buf[bufferIndex];
          else
            break;
        }
        while (!can.write(msg));  // 送信

      }
    }

    uint8_t& operator[](uint8_t index) const {
      return buf[index];
    }
    uint8_t size() const {
      return writeSize;
    }

    void show(const String& str = "") const {
      for (uint8_t i = 0; i < writeSize; i++)
        Serial.print(buf[i]), Serial.print('\t');
      Serial.print(str);
    }

};

// static メンバ初期化
uint8_t CanSlaveWriter::MASTER_ID = 255;
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_256> CanSlaveWriter::can;
