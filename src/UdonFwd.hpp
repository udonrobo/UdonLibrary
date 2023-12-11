//
//    UdonLibrary
// 
//    Copyright (c) 2022-2023 udonrobo
//

//
//    ヘッダーを一括でインクルードしたくない場合、代わりにこのファイルをインクルードし、
//    必要なヘッダーファイルを個別にインクルードしてください。(このファイルをインクルードするのはメインファイルからのみでOK)
//
//    (Arduino IDE はライブラリのサブディレクトリにあるヘッダーを直接インクルードすることがでないため)
//
//    ex: Master.ino
//    #include <UdonFwd.hpp>
//    #include <Udon/Driver/Imu.hpp>
//
//    ex: Steering.hpp
//    #include <Udon/Algorithm/PidController.hpp>
//


#pragma once


// UdonLibrary が含まれているかどうかを判定するマクロ
#define UDON_INCLUDED
