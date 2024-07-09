//
//    thirdparty ライブラリとの衝突を防ぐテスト
//
//    Copyright (c) 2022-2024 udonrobo
//

// ローカル環境でテストする場合、ライブラリがインストールされている場合のみテストする
// ライブラリがインストールされていない場合、テストをスキップする

#if __has_include(<Adafruit_BusIO_Register.h>)

#    include <Arduino.h>

#    include <Udon.hpp>

// 以下サードパーティーライブラリ
// 本ライブラリに改変したサードパーティーライブラリが含まれているため、既にライブラリディレクトリにサードパーティーライブラリが存在する場合、多重定義エラーが発生する可能性が考えられる。
// エラー時はインライン化や名前空間の使用などで対応する。
#    include <Adafruit_BusIO_Register.h>
#    include <Adafruit_Sensor.h>
#    include <Adafruit_BNO055.h>

#endif