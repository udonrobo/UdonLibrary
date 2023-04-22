/*
  小数のみ(10桁程度)
  デフォルトではparameter.scvというファイルで保存される。読み取りと書き込みは同じファイル

  @file   parameter.hpp
  @date   2023/01/08
  @brief  パラメータをSDカードに保存する
  @author 前田 朋樹

  開発環境:Teensy4.1
*/

#pragma once
#include <Arduino.h>
#include <SdFat.h>

namespace udon {

class parameter {
  private:
    SdFs sd;
    FsFile dataFile;
    char *name;
    bool Card_Insertion;
    int size;
    double *readData;
    double *writeData;
  public:
    parameter(int dataSize, char *fileName = (char *)"parameter.csv") {
      Card_Insertion = sd.begin(SdioConfig(FIFO_SDIO));
      size = dataSize;
      name = fileName;
      readData = new double[size];
      writeData = new double[size];
    }
    //書き込み
    void setParameter(int dataNum, double data) {
      writeData[dataNum - 1] = data;
    }
    void writeParameter() {
      if (Card_Insertion) {
        dataFile.open(name, O_RDWR);
        dataFile.seek(dataFile.size());  //ファイルの最後に移動
        for (int i = 0; i < size; i++) {
          dataFile.print(writeData[i]);
          dataFile.print(",");
        }
        dataFile.println();
        dataFile.close();
      }
    }
    //読み取り
    void readParameter() {
      if (Card_Insertion) {
        dataFile.open(name, O_RDWR);
        String str;
        while (dataFile.available()) {
          str = dataFile.readStringUntil('\n', size * 15);
        }
        char c[size * 15];
        str.toCharArray(c, size * 15);
        char tmp[15];
        int32_t i = 0, j = 0, k = 0;
        while (k < size) {
          if (c[i] == '\0' || c[i] == ',') {
            tmp[j] = '\0';
            readData[k++] = atof(tmp);
            j = 0;
          } else
            tmp[j++] = c[i];
          i++;
        }
        dataFile.close();
      } else {
        for (int i = 0; i < size; i++)
          readData[i] = 0.0;
      }
    }
    double getParameter(int dataNum)const {
      return readData[dataNum - 1];
    }
    void readShow(char end = {})const {
      for (int i = 0; i < size; i++) {
        Serial.print(readData[i]);
        Serial.print("\t");
      }
      Serial.print(end);
    }
    void writeShow(char end = {})const {
      for (int i = 0; i < size; i++) {
        Serial.print(writeData[i]);
        Serial.print("\t");
      }
      Serial.print(end);
    }
};
}
