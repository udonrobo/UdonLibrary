//-------------------------------------------------------------------
//
//	  UdonLibrary
// 
//	  Copyright (c) 2022-2023 udonrobo
//
//	  Licensed under the MIT License.
//
//-------------------------------------------------------------------
//
//    ヘッダーを一括でインクルードしたくない場合、
//    代わりにこのファイルをインクルードし、
//    必要なヘッダーファイルを個別にインクルードしてください。(メインファイルからのみでOK)
//
//    (Arduino IDE はライブラリのサブディレクトリにあるヘッダーを直接インクルードすることがでないため)
//
//    ex: Master.ino
//    #include <UdonFwd.hpp>
//    #include <udon/sencor/Imu.hpp>
//
//    ex: Steering.hpp
//    #include <udon/algorithm/PidController.hpp>
//
//-------------------------------------------------------------------


#pragma once


// UdonLibrary が含まれているかどうかを判定するマクロ
#define UDON_INCLUDED
