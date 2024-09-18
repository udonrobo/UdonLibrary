#pragma once

namespace Udon
{

    namespace Impl
    {

        /// @brief E220 ベースクラス
        /// @note 送受信者とも初期化内容が同じであったため共通化
        class E220Base
        {
        public:
            /// @brief 設定
            struct Config
            {
                HardwareSerial& serial;    // シリアルポート

                uint8_t m0;     // M0 ピン
                uint8_t m1;     // M1 ピン
                uint8_t aux;    // AUX ピン

                /// @brief チャンネル
                /// @note 0-12 (13-30も使用できますが通信速度が遅くなります)
                /// @note 相手と合わせる
                uint8_t channel = 0;

                /// @brief アドレス
                /// @note 0x0000-0xFFFD
                /// @note 相手と合わせる
                uint16_t address = 0x0102;

#if __cplusplus < 201402L

                /// @brief コンストラクタ
                /// @note C++11 ではメンバ変数にデフォルト値を持たせたとき、別の値で初期化することができないため、コンストラクタを用意 (#^ω^)
                Config(HardwareSerial& serial, uint8_t m0, uint8_t m1, uint8_t aux, uint8_t channel = 0, uint16_t address = 0x0102)
                    : serial(serial)
                    , m0(m0)
                    , m1(m1)
                    , aux(aux)
                    , channel(channel)
                    , address(address)
                {
                }
#endif
            };

            /// @brief 現在の設定を取得
            /// @note チャンネル書き換え等に使用します。
            /// @note begin() 呼び出し前に書き換えを行ってください。
            Config& getConfigReference()
            {
                return config;
            }

        protected:
            Config config;

            /// @brief コンストラクタ
            /// @param config 設定
            E220Base(const Config& config)
                : config(config)
            {
            }

            /// @brief 通信開始
            void begin()
            {
                pinMode(config.m0, OUTPUT);
                pinMode(config.m1, OUTPUT);
                pinMode(config.aux, INPUT);

                // E220 はデフォルトボーレートが9600なのでそちらに合わせる
                config.serial.begin(9600);

                // 設定モードに移行
                digitalWrite(config.m0, HIGH);
                digitalWrite(config.m1, HIGH);

                delay(100);

                // レジスタを書き換え
                const uint8_t registerMap[]{
                    /* [0x00] */ static_cast<uint8_t>(config.address >> 8),    // アドレス上位バイト
                    /* [0x01] */ static_cast<uint8_t>(config.address >> 0),    // アドレス下位バイト
                    /* [0x02] */ 0b11100001,                                   // [7-5]UART通信レート / [4-0]伝送レート
                    /* [0x03] */ 0b00000001,                                   // [7-6]サブパケット長 / [5]RSSI有効 / [4-2]未使用(予約) / [1-0]送信出力電力
                    /* [0x04] */ config.channel,                               // 周波数チャンネル (920.8 MHz + CH×200kHz)
                    /* [0x05] */ 0b11000011,                                   // [7]RSSI バイト / [6]送信方法 / [5-3]未使用(予約) / [2-0]WOR サイクル
                };
                config.serial.write(static_cast<uint8_t>(0xc0));
                config.serial.write(static_cast<uint8_t>(0x00));
                config.serial.write(static_cast<uint8_t>(sizeof registerMap));
                config.serial.write(registerMap, sizeof registerMap);

                delay(100);

                // レスポンスを破棄
                while (config.serial.available())
                    (void)config.serial.read();

                // 通常モードに移行
                digitalWrite(config.m0, LOW);
                digitalWrite(config.m1, LOW);

                delay(100);

                config.serial.end();
                config.serial.begin(115200);
            }
        };

    }    // namespace Impl

}    // namespace Udon