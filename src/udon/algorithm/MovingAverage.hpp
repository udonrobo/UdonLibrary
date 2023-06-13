/**
        @file   MovingAverage.hpp
        @brief  移動平均クラス
        @author 谷川　豊章
        @date   2016/9/15
*/

#ifndef DEF_MovingAverage_H
#define DEF_MovingAverage_H

#include <math.h>

namespace udon
{

    /**     移動平均クラス
     */
    class MovingAverage
    {

        int  dataSize;    ///< 平均を取るデータのサイズ
        int* data;        ///< 平均を取るデータ

        int writeIndex;    ///< 次の値のdataへの書き込み場所

        long sum;    /// dataに保存されているデータの合計値

    public:
        /**     コンストラクタ
                @param  dataSize    [in]移動平均するデータの個数
        */
        MovingAverage(size_t dataSize)
            : dataSize(fmax(dataSize, 1ULL))
            , data(new int[dataSize]())
            , writeIndex()
            , sum()
        {
        }

        /**     ディストラクタ
         */
        ~MovingAverage() noexcept
        {
            delete[] data;
        }

        /**     値の更新
                @param  value   [in]移動平均するデータ
        */
        void update(int value) noexcept
        {
            sum -= data[writeIndex];       // 取り除くデータの値分だけ合計から取り除く
            data[writeIndex++] = value;    // データを更新する
            sum += value;                  // 合計値に更新した値を追加する

            if (writeIndex >= dataSize)
                writeIndex = 0;
        }

        /**     平均値の取得
                @return     平均値
        */
        double getValue() const noexcept
        {
            return double(sum) / double(dataSize);
        }
    };

}    // namespace udon

#endif
