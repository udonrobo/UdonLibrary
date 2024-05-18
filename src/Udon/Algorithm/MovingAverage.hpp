//
//    移動平均クラス
//
//    Copyright (c) 2016-2023 谷川 豊章
//    Copyright (c) 2016-2023 udonrobo
//

#ifndef DEF_MovingAverage_H
#define DEF_MovingAverage_H

namespace Udon
{

    /// @brief 移動平均クラス
    template <size_t DataSize>
    class MovingAverage
    {

        int data[DataSize];    ///< 平均を取るデータ

        size_t writeIndex;    ///< 次の値のdataへの書き込み場所

        long sum;    /// dataに保存されているデータの合計値

    public:
        /// @brief コンストラクタ
        /// @param DataSize [in]移動平均するデータの個数
        MovingAverage()
            : data()
            , writeIndex()
            , sum()
        {
        }

        /// @brief 値の更新
        /// @param value [in]移動平均するデータ
        void update(int value) noexcept
        {
            sum -= data[writeIndex];       // 取り除くデータの値分だけ合計から取り除く
            data[writeIndex++] = value;    // データを更新する
            sum += value;                  // 合計値に更新した値を追加する

            if (writeIndex >= DataSize)
                writeIndex = 0;
        }

        /// @brief 平均値の取得
        /// @return 平均値
        double getValue() const noexcept
        {
            return static_cast<double>(sum) / DataSize;
        }

        /// @brief 値の更新と平均値の取得
        /// @param value [in]移動平均するデータ
        /// @return 平均値
        double operator()(double value) noexcept
        {
            update(value);
            return getValue();
        }
    };

}    // namespace Udon

#endif
