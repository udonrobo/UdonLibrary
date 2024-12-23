//
//    速度型PIDコントローラクラス
//

#ifndef DEF_SpeedPidController_H
#define DEF_SpeedPidController_H

#include "Math.hpp"
#include <Udon/Types/Optional.hpp>
#include <Udon/Types/Range.hpp>

namespace Udon
{
    /// @brief 速度型PID制御器
    class SpeedPidController
    {
    public:
        struct Parameter
        {
            double p;
            double i;
            double d;
        };

    private:
        Parameter constant;    ///< 係数

        Udon::Optional<Parameter> requestConstant;    ///< 一周期だけ適用する係数

        Parameter power;    ///< 操作量

        double lastPowerPro;    ///< 過去の操作量
        double lastError;       ///< 過去の偏差
        double rowPassDif;
        double output;              ///< 出力
        const double MAX_POWER;     /// 最大値
        const double INTERVAL_S;    ///< updateの呼び出される周期(unit:sec)

    public:
        /// @brief コンストラクタ
        /// @param  kPro            比例係数
        /// @param  kInt            積分係数
        /// @param  kDif            微分係数
        /// @param  callInterval_us update()の呼び出し周期
        SpeedPidController(double kPro, double kInt, double kDif,
                           unsigned long callInterval_us,
                           double maxPower = 10000) noexcept
            : constant{ kPro, kInt, kDif }
            , requestConstant{}
            , power{}
            , lastPowerPro(0.0)
            , lastError()
            , rowPassDif()
            , output()
            , MAX_POWER(maxPower)
            , INTERVAL_S(callInterval_us / 1000000.0)
        {
        }

        /// @brief データ更新
        /// @param controlValue 制御量
        /// @param targetValue 目標値
        /// @return 操作量
        void update(double controlValue, double targetValue) noexcept
        {
            // 係数の適用
            const auto coefficient = requestConstant ? *requestConstant : constant;
            requestConstant.reset();

            // 偏差の計算
            const double error = targetValue - controlValue;

            // 比例量の計算
            power.p = (error - lastError) / INTERVAL_S * coefficient.p;

            // 積分量の計算
            power.i = error * coefficient.i;

            // 微分量の計算
            power.d = (power.p - lastPowerPro) / INTERVAL_S;

            // ローパスフィルタの計算
            rowPassDif += (power.d - rowPassDif) / 8 * coefficient.d;

            const double deltaPower = power.p + power.i + rowPassDif;

            output = Udon::Constrain(output + deltaPower, -MAX_POWER, MAX_POWER);

            // 偏差の保存
            lastPowerPro = power.p;
            lastError = error;
        }

        /// @brief 操作量の取得
        /// @return 操作量
        double getPower() const noexcept
        {
            return output;
        }

        /// @brief 操作量の取得
        /// @param min 操作量の最小値
        /// @param max 操作量の最大値
        /// @return 操作量
        double getPower(double min, double max) const noexcept
        {
            return Udon::Constrain(getPower(), min, max);
        }

        /// @brief 操作量の取得
        /// @param min 操作量の最小値
        /// @param max 操作量の最大値
        /// @return 操作量
        double getPower(double min, double max) const noexcept
        {
            return Udon::Constrain(getPower(), min, max);
        }

        /// @brief 更新、操作量の取得
        /// @param controlValue 制御量
        /// @param targetValue 目標値
        /// @return 操作量
        double operator()(double controlValue, double targetValue) noexcept
        {
            update(controlValue, targetValue);
            return getPower();
        }

        /// @brief 更新、操作量の取得
        /// @param controlValue 制御量
        /// @param targetValue 目標値
        /// @param min 操作量の最小値
        /// @param max 操作量の最大値
        /// @return 操作量
        double operator()(double controlValue, double targetValue, double min,
                          double max) noexcept
        {
            update(controlValue, targetValue);
            return getPower(min, max);
        }

        /// @brief 更新、操作量の取得
        /// @param controlValue 制御量
        /// @param targetValue 目標値
        /// @param range 操作量の範囲
        /// @return 操作量
        double operator()(double controlValue, double targetValue, const Udon::Range<double>& range) noexcept
        {
            return operator()(controlValue, targetValue, range.min, range.max);
        }

        /// @brief 操作量のクリア
        /// @note 内部の量をすべて0にする。
        void clearPower() noexcept
        {
            power = {};
            output = 0.0;
            lastPowerPro = 0.0;
            rowPassDif = 0.0;
            lastError = 0.0;
        }
        /// @brief 一周期のみ適用する比例係数の設定
        /// @param value 係数
        void requestParamPro(double value) noexcept
        {
            if (requestConstant)
            {
                requestConstant->p = value;
            }
            else
            {
                requestConstant = Parameter{ value, constant.i, constant.d };
            }
        }

        /// @brief 一周期のみ適用する積分係数の設定
        /// @param value 係数
        void requestParamInt(double value) noexcept
        {
            if (requestConstant)
            {
                requestConstant->i = value;
            }
            else
            {
                requestConstant = Parameter{ constant.p, value, constant.d };
            }
        }

        /// @brief 一周期のみ適用する微分係数の設定
        /// @param value 係数
        void requestParamDif(double value) noexcept
        {
            if (requestConstant)
            {
                requestConstant->d = value;
            }
            else
            {
                requestConstant = Parameter{ constant.p, constant.i, value };
            }
        }

        /// @brief 一周期のみ適用する係数の設定
        /// @param value 係数
        void requestParam(const Parameter& value) noexcept
        {
            requestConstant = value;
        }

        /// @brief 比例係数の設定
        /// @param value 比例係数
        void setParamPro(double value) noexcept { constant.p = value; }

        /// @brief 積分係数の設定
        /// @param value 積分係数
        void setParamInt(double value) noexcept { constant.i = value; }

        /// @brief 微分係数の設定
        /// @param value 微分係数
        void setParamDif(double value) noexcept { constant.d = value; }

        /// @brief 係数の設定
        /// @param value 係数
        void setParam(const Parameter& value) noexcept { constant = value; }

        /// @brief 比例係数の取得
        /// @return 比例係数
        double getParamPro() const noexcept { return constant.p; }

        /// @brief 積分係数の取得
        /// @return 積分係数
        double getParamInt() const noexcept { return constant.i; }

        /// @brief 微分係数の取得
        /// @return 微分係数
        double getParamDif() const noexcept { return constant.d; }

        /// @brief 係数の取得
        /// @return 係数
        const Parameter& getParam() const noexcept { return constant; }

        /// @brief 比例量の取得
        /// @return 比例量
        double getPowerPro() const noexcept { return power.p; }

        /// @brief 積分量の取得
        /// @return 積分量
        double getPowerInt() const noexcept { return power.i; }

        /// @brief 微分量の取得
        /// @return 微分量
        double getPowerDif() const noexcept { return power.d; }
    };
}    // namespace Udon

#endif
