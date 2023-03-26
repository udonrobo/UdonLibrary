#pragma once
#include <arduino.h>
#include <Wire.h>

namespace udon
{

class MasterWriter {
  private:
    int size;
    int address;
    byte* data;
  public:
    MasterWriter(int Address, int array_size) : address(Address), size(array_size) {
      data = new byte[size];
      for (int i = 0; i < array_size; i++)
        data[i] = 0;
    }

    //デストラクタ
    virtual ~MasterWriter() {
      delete[] data;
    }

    void SetBitData(int array_num, byte bit_num, bool bit_data) {
      bitWrite(data[array_num - 1], bit_num, bit_data);
    }

    void SetMotorData(byte port_num, int16_t pwm) {
      SetBitData(size, port_num - 1, ((pwm < 0) ? 0 : 1)); //モーターは五つのデータ(pwm×4、方向1つ)をやりくりする。この行では５番目のデータに方向(0か1)を代入　port_num1～4　size5 方向のビットは最後に送られる(一番値が大きい)
      SetSingleData(port_num, abs(pwm)); //port_numの数字が小さい順にpwmが送られる
    }

    void SetSingleData(int array_num, byte single_data) {
      data[array_num - 1] = single_data;
    }

    void Update() {
      Wire.beginTransmission(address);
      Wire.write(data, size);
      Wire.endTransmission();
    }

    void reset() {
      for (int i = 0; i < size; i++)
        data[i] = 0;
    }

    void m_reset() {
      for (int i = 0; i < size - 1; i++)
        data[i] = 0;
    }

    void show() const {
      for (int i = 0; i < size; i++) {
        Serial.print(data[i]);
        Serial.print("\t");
      }
    }
};
}
