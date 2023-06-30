//-------------------------------------------------------------------
//
//    UdonLibrary
//
//    Copyright (c) 2016-2023 谷川 豊章
//    Copyright (c) 2016-2023 udonrobo
//
//    Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    移動平均クラス
//
//-------------------------------------------------------------------

#ifndef DEF_MovingAverage_H
#define DEF_MovingAverage_H

#include <Udon/Math/Math.hpp>

namespace Udon
{

    /**     移動平均クラス
     */
    class MovingAverage
    {

        size_t dataSize;    ///< 平均を取るデータのサイズ
        int*   data;        ///< 平均を取るデータ

        size_t writeIndex;    ///< 次の値のdataへの書き込み場所

        long sum;    /// dataに保存されているデータの合計値

    public:
        /**     コンストラクタ
                @param  dataSize    [in]移動平均するデータの個数
        */
        MovingAverage(size_t dataSize)
            : dataSize(Udon::Max(dataSize, 1ULL))
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

}    // namespace Udon

#endif
