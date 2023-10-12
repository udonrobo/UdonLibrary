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
//    PIDコントローラクラス
//
//-------------------------------------------------------------------

#ifndef DEF_PidController_H
#define DEF_PidController_H

#include "Math.hpp"
#include <Udon/Stl/Optional.hpp>

namespace Udon
{

    /// @brief PID制御器
    class PidController
    {
        struct Parameter
        {
            double p;
            double i;
            double d;
            void clear() noexcept { p = i = d = 0.0; }
        };

        Parameter constant;    ///< 係数

        Udon::Optional<Parameter> requestConstant;    ///< 一周期だけ適用する係数

        Parameter power;    ///< 操作量

        double lastError;    ///< 過去の偏差

        const double MAX_INT_POWER;    ///< 積分量の最大値
        const double INTERVAL_S;       ///< updateの呼び出される周期(unit:sec)

    public:
        /// @brief コンストラクタ
        /// @param  kPro            比例係数
        /// @param  kInt            積分係数
        /// @param  kDif            微分係数
        /// @param  callInterval_us update()の呼び出し周期
        /// @param  maxIntPower     積分量の最大値
        PidController(double kPro, double kInt, double kDif, unsigned long callInterval_us, double maxIntPower = 1023.0) noexcept
            : constant{ kPro, kInt, kDif }
            , requestConstant{}
            , power{}
            , lastError()
            , MAX_INT_POWER(maxIntPower)
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
            power.p = coefficient.p * error;

            // 積分量の計算
            power.i += coefficient.i * error * INTERVAL_S;
            power.i = Udon::Constrain(power.i, -MAX_INT_POWER, MAX_INT_POWER);

            // 微分量の計算
            power.d = coefficient.d * (error - lastError) / INTERVAL_S;

            // 偏差の保存
            lastError = error;
        }

        /// @brief 操作量の取得
        /// @param min 操作量の最小値
        /// @param max 操作量の最大値
        /// @return 操作量
        double getPower(double min = -250, double max = 250) const noexcept
        {
            return Udon::Constrain(power.p + power.i + power.d, min, max);
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

        /// @brief 操作量のクリア
        /// @remark 内部の量をすべて0にする。
        void clearPower() noexcept
        {
            power     = {};
            lastError = 0.0;
        }


        /// @brief 一周期のみ適用する比例係数の設定
        /// @param value 係数
        void requestParamPro(double value) noexcept { requestConstant = Parameter{ value, constant.i, constant.d }; }

        /// @brief 一周期のみ適用する積分係数の設定
        /// @param value 係数
        void requestParamInt(double value) noexcept { requestConstant = Parameter{ constant.p, value, constant.d }; }

        /// @brief 一周期のみ適用する微分係数の設定
        /// @param value 係数
        void requestParamDif(double value) noexcept { requestConstant = Parameter{ constant.p, constant.i, value }; }

        /// @brief 一周期のみ適用する係数の設定
        /// @param value 係数
        void requestParam(const Parameter& value) noexcept { requestConstant = value; }


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
