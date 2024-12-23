#pragma once

#ifdef ARDUINO

#    include "Stopwatch.hpp"

/*
 * 長さと旋回角の台形制御をして、ベジェ曲線の経路クラスと合わせて使用することを目的とした。
 */

namespace Udon
{

    class Trapezoidal
    {
    private:
        double upTime, maxPowerTime, downTime;
        double acc, maxSpeed, limitSpeed;
        bool nextState, dir;
        double nowPos, nowSpeed;
        double oldTarget;
        double startSpeed, endSpeed;
        double nextNowPos, nextNowSpeed;
        Stopwatch Stopwatch;
        bool reCalculation;

    public:
        /// @brief 一次元台形制御のコンストラクタ
        /// @param acc m/s^2 加速度
        /// @param maxSpeed m/s 最大速度
        /// @param nowPos mm スタート座標
        Trapezoidal(double acc, double maxSpeed, double nowPos)
            : upTime()
            , maxPowerTime()
            , downTime()
            , acc(acc * 0.001)      // mm/ms^2 == (m/s^2)*0.001
            , maxSpeed(maxSpeed)    // mm/ms == m/s
            , limitSpeed()
            , nextState(true)
            , dir()
            , nowPos(nowPos)
            , nowSpeed()
            , oldTarget(nowPos)
            , startSpeed()
            , endSpeed()
            , nextNowPos(nowPos)
            , nextNowSpeed()
            , Stopwatch()
            , reCalculation()
        {
        }
        // 入力はs単位で計算時はms単位にする

        /// @brief 目標値を計算する
        /// @param target mm 目標値
        /// @param endS mm/s 巡行後の終了スピード(デフォルトで0mm/s)
        double operator()(const double target, bool isStop = false, const double endS = 0.0)
        {
            // X=Vot+(1/2)*at^2;
            // V=a*t +Vo;
            // V^2-Vo^2 =2as;
            if (isStop)
            {
                // 非常停止時
                Stopwatch.stop();
                reCalculation = true;
                nextNowSpeed = 0;
            }
            if (oldTarget != target || reCalculation)
            {                         // 目標座標が変わったときは計算しなおす
                Stopwatch.start();    // タイマーを開始する←何度呼び出しても一度だけ実行される
                nextState = false;
                oldTarget = target;
                reCalculation = false;

                nowPos = nextNowPos;          // 途中までの経路情報の保持
                startSpeed = nextNowSpeed;    // 最初のスピードは前の経路情報をほじ
                endSpeed = endS * 0.001;
                const double distance = target - nowPos;    // 距離を求める
                dir = (distance > 0.0 ? 1 : 0);             // 増減の方向を決める

                endSpeed = abs(endSpeed) * (dir ? 1 : -1);
                maxSpeed = abs(maxSpeed) * (dir ? 1 : -1);
                acc = abs(acc) * (dir ? 1 : -1);

                upTime = (maxSpeed - startSpeed) / acc;    // 最高速度までの加速にかかる時間 ms
                downTime = (maxSpeed - endSpeed) / acc;    // 減速にかかる時間 ms
                if (upTime < 0.0)                          // 今と逆に進みたいときに減速分を増加させるため
                    downTime -= upTime;

                const double L1 = startSpeed * upTime + acc * sq(upTime) * 0.5;      // 加速時における移動距離 mm
                const double L3 = endSpeed * downTime + acc * sq(downTime) * 0.5;    // 減速時における移動距離 mm
                if (abs(L1) + abs(L3) > abs(distance))
                {    // 台形ができなくなり、三角形になるときの制御
                    limitSpeed = 2.00 * acc * distance * 0.5 + sq(startSpeed);
                    limitSpeed = sqrt(limitSpeed) * (dir ? 1 : -1);    // 必ず＋になる
                    if (abs(limitSpeed) < abs(startSpeed))
                        limitSpeed = startSpeed;
                    upTime = (limitSpeed - startSpeed) / acc;
                    downTime = (limitSpeed - endSpeed) / acc;
                    maxPowerTime = 0.0;    // limitSpeed,upTimeがマイナスになることもある
                }
                else
                {                                                      // 台形が作れる場合の制御
                    maxPowerTime = (distance - L1 - L3) / maxSpeed;    // 最高速度での移動時間 ms
                    limitSpeed = maxSpeed;
                }
                // 目標値がずれることがあった↓
                // 目標値を変えた際の速度によって確定で行き過ぎる場合があり、
                // その場合に逆に進んでいないから目標値がずれてる
            }

            double ut = constrain(Stopwatch.getTime(), 0.00, upTime);
            double mt = constrain(Stopwatch.getTime() - upTime, 0.00, maxPowerTime);
            double dt = constrain(Stopwatch.getTime() - (upTime + maxPowerTime), 0.00, downTime);
            double targets = acc * sq(ut) * 0.50 + startSpeed * ut + maxSpeed * mt + (-acc * sq(dt) * 0.50) + limitSpeed * dt;

            if (!nextState)
            {    // 経路を巡行してないときだけ更新する
                nextNowSpeed = startSpeed + acc * ut + -acc * dt;
                nextNowPos = targets + nowPos;
            }
            if (upTime + downTime + maxPowerTime <= Stopwatch.getTime())
            {
                nowPos = nextNowPos;    // 一つの経路を巡行し終えた時の座標の情報を保持
                Stopwatch.reset();
                Stopwatch.stop();
                nextState = true;
                oldTarget = nowPos;    // 行き過ぎた場合にもう一度計算するようにする
            }
            return nextNowPos;
        }

        /// @brief 目標値計算用の時間の取得
        /// @return 時間 ms
        double getTime() const
        {
            return Stopwatch.getTime();
        }
        /// @brief 実行状態を取得する
        /// @return セットされている経路を巡行し終えたらtrue
        bool status() const
        {
            return nextState;
        }
        /// @brief 次の経路情報を入力後必ず呼び出す必要あり
        void setTarget()
        {
            reCalculation = true;
        }
        /// @brief 最終の開始座標を取得する
        /// @return 開始座標 mm
        double getNowPos() const
        {
            return nowPos;
        }
        /// @brief 台形制御の現在座標を取得する
        /// @return 計算座標 mm
        double getTargetPos() const
        {
            return nextNowPos;
        }
        /// @brief 現在速度を取得する
        /// @return 現在速度 m/s
        double getNowSpeed() const
        {
            return nowSpeed * 1000;
        }
        void setAcc(double acc)
        {
            this->acc = acc * 0.001;
        }
        void posReset()
        {
            nextNowPos = 0;
            nowPos = 0;
        }
    };
}    // namespace Udon
#endif