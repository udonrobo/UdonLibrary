#pragma once

#include "CanBase_t4.h"
#include <FlexCAN_T4.h>

class CanMasterWriter : private CanBase {

    const uint8_t targetID;   // 送信先
    const uint8_t writeSize;  // 送信サイズ
    uint8_t* buf;             // 送信バッファ

  public:

    CanMasterWriter(uint8_t targetID, uint8_t writeSize)
      : targetID(targetID)
      , writeSize(writeSize)
      , buf(new uint8_t[writeSize])
    {
      memset(buf, 0, writeSize);
      CanBase::begin();
    }
    ~CanMasterWriter(){
      delete[] buf;
    }

    void update() {
      const uint8_t paketCount = ceil(writeSize / 6.);  // 送信回数
      for (uint8_t i = 0; i < paketCount; i++) {        // パケットに分けて送信

        CAN_message_t msg;
        msg.id     = MASTER_ID;
        msg.buf[0] = targetID;
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
