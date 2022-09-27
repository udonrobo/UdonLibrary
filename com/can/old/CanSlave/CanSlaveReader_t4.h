#pragma once

#include "CanBase_t4.h"

#include <FlexCAN_T4.h>
#include <IntervalTimer.h>

class CanSlaveReader : private CanBase {

    static IntervalTimer timer;
    static uint8_t myID;
    static uint8_t readSize;
    static uint8_t *buf;
    static uint32_t lastReceiveMs;

  public:

    CanSlaveReader(uint8_t myID, uint8_t readSize) {
      CanSlaveReader::myID = myID;
      CanSlaveReader::readSize = readSize;
      CanSlaveReader::buf = new uint8_t[readSize];
      memset(buf, 0, readSize);
      CanBase::begin();
      can.onReceive(onReceive);
      timer.begin([] {can.events();}, 200);
    }
    ~CanSlaveReader() {
      delete[] buf;
    }

    /**
      @brief 通信状態を判定する
      @return 通信中:true 未通信:false
    */
    bool isConnected() {
      return millis() - lastReceiveMs < 100;
    }
    operator bool() {
      return millis() - lastReceiveMs < 100;
    }

    /**
       @brief getter
    */
    const uint8_t& operator[](uint8_t index) const {
      return buf[index];
    }
    uint8_t size() const {
      return readSize;
    }

    /**
       @brief print
    */
    void show(const String& str = "") const {
      for (uint8_t i = 0; i < readSize; i++)
        Serial.print(buf[i]), Serial.print('\t');
      Serial.print(str);
    }

  private:
    static void onReceive(const CAN_message_t& msg) {
      // communication format [8byte]
      // [target address][packet index][data][data][data][data][data][data]

      const uint8_t writerID = msg.id;      // 送信元ID
      const uint8_t targetID = msg.buf[0];  // ターゲットID

      if (writerID == MASTER_ID && targetID == myID) { // マスターから自身に向けた送信

        const uint8_t packetIndex = msg.buf[1];  // 分割したデータの要素番号

        for (uint8_t i = 0; i < 6; i++) {
          const uint8_t bufferIndex = i + packetIndex * 6;
          if (bufferIndex < readSize)  // 配列範囲内参照時
            buf[bufferIndex] = msg.buf[i + 2/*先頭2byteを飛ばす*/];
          else
            break;
        }

      }
      lastReceiveMs = millis();
    }
};

IntervalTimer CanSlaveReader::timer;
uint8_t CanSlaveReader::myID;
uint8_t CanSlaveReader::readSize;
uint8_t *CanSlaveReader::buf;
uint32_t CanSlaveReader::lastReceiveMs;
