/**
        @file   MovingAverage.h
        @brief  移動平均クラス
        @auther 谷川　豊章
        @date   2016/9/15
*/


#ifndef DEF_MovingAverage_H
#define DEF_MovingAverage_H

#include <arduino.h>

/**     移動平均クラス
*/
class MovingAverage {

    int* data;      ///< 平均を取るデータ
    int dataSize;   ///< 平均を取るデータのサイズ

    int writeIndex; ///< 次の値のdataへの書き込み場所

    long sum;       ///dataに保存されているデータの合計値

  public:

    /**     コンストラクタ
            @param  dataSize    [in]移動平均するデータの個数
    */
    MovingAverage(int dataSize) : dataSize( max(dataSize, 1) ), writeIndex(0), sum(0)
    {
      data = new int[dataSize];
      for (int i = 0; i < dataSize; i++) {
        data[i] = 0;
      }
    }

    /**     ディストラクタ
    */
    ~MovingAverage() {
      delete[] data;
    }

    /**     値の更新
            @param  value   [in]移動平均するデータ
    */
    void Update(int value) {
      sum -= data[writeIndex];    //取り除くデータの値分だけ合計から取り除く
      data[writeIndex++] = value; //データを更新する
      sum += value;               //合計値に更新した値を追加する

      if (writeIndex >= dataSize)
        writeIndex = 0;
    }

    /**     平均値の取得
            @return     平均値
    */
    double GetValue() {
      return double(sum) / double(dataSize);
    }
};


#endif
