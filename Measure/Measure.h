/// @file   Measure.h
/// @date   2022/09/22
/// @brief  計測輪の入力を座標に変換するクラス
/// @author 大河 祐介

#pragma once

#include "EncoderBoardTeensy.h"
#include "Gyro.h"

class Measure : private Gyro {
	public:
		template<class T>
		struct Vec2 {
			T x, y;
		};
	private:
		EncoderBoardTeensy& enc;

		static constexpr double ppr = 8192;       /// 分解能
		static constexpr double diameter = 50.3;  /// 計測輪直径

		struct Wheel {  /// 計測輪情報
			const uint8_t port;
			int32_t count;
		} wheelx, wheely;

		Vec2<double> pos;
		Vec2<bool>   dir;

	public:
		/// @param address EncoderBoardTeensyクラスのインスタンス参照
		/// @param portX   X軸のエンコーダーの入力ポート
		/// @param portY   Y軸のエンコーダーの入力ポート
		Measure(EncoderBoardTeensy& enc, const uint8_t portX, const uint8_t portY)
			: enc(enc)
			, wheelx{portX}
			, wheely{portY}
			, pos{}
			, dir{ true, true }
		{}

		/// @brief 各物理情報を計算する
		/// @note  必須
		void update() {
			const auto prevX = wheelx.count;
			const auto prevY = wheely.count;
			wheelx.count = enc.getCount(wheelx.port);
			wheely.count = enc.getCount(wheely.port);

			/// 相対座標の変化量算出
			constexpr double rawToMillimeter = diameter * PI / ppr;
			const Vec2<double> delta {
				(wheelx.count - prevX) * rawToMillimeter,
				(wheely.count - prevY) * rawToMillimeter
			};

			/// 回転行列を用いて絶対座標に変換
			const double yaw = radians(Gyro::yaw());
			pos.x += delta.x * cos(yaw) - delta.y * sin(yaw);
			pos.y += delta.x * sin(yaw) + delta.y * cos(yaw);
		}

		/// @brief 消去
		/// @param yaw 初期の旋回角
		void clear(const double yaw = 0) {
			pos = {};
			Gyro::clear(yaw);
		}
		void setDir(const Vec2<bool>& direction) {
			dir = direction;
		}

		/// @brief 絶対座標を取得
		double x() const {
			return pos.x * (dir.x ? 1 : -1);
		}
		double y() const {
			return pos.y * (dir.y ? 1 : -1);
		}

		/// @brief 表示
		void show(const char end = {}) const {
			Serial.print(x()), Serial.print('\t');
			Serial.print(y()), Serial.print('\t');
			Serial.print(Gyro::yaw()), Serial.print(end);
		}
};
