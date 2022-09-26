/**
        @file   LoopCycleController.h
        @brief  ループ周期を一定にする
        @author 谷川　豊章
        @date   2016/8/11
*/


#ifndef DEF_LoopCycleController_H
#define DEF_LoopCycleController_H

#include <arduino.h>

/**     LoopCycleControllerクラス
        ループ周期をコントロールするクラス
*/
class LoopCycleController {
    const unsigned long CYCLE_US;   ///< 周期(unit:us)
    unsigned long       lastTime;   ///< 前回のループ開始時間(unit:us)

    bool    isError;    ///< 指定された周期を超過しているときtrue

  public:

    /**     コンストラクタ
            @param  cycle_us    [in]目標のループ周期
    */
    LoopCycleController(unsigned long cycle_us) : CYCLE_US(cycle_us), lastTime(0), isError(false)
    { }

    /**     更新
            ループ周期が一定になるように待つ
    */
    void Update() {
      if ( micros() - lastTime > CYCLE_US ) { //指定されたループ周期より処理に時間がかかっている

        if (lastTime != 0) { //初回のループでなければエラーフラグを立てる
          isError = true;
        }
        lastTime = micros();

      } else {

        //一定周期になるように待つ
        isError = false;
        while ( micros() - lastTime < CYCLE_US );
        lastTime += CYCLE_US;

      }
    }

    /**     ループ周期の読み出し
            @return ループ周期
    */
    unsigned long Cycle_us() {
      return CYCLE_US;
    }

    /**     エラー検出
            @retval false   ループ周期は一定
            @retval true    ループ周期は一定でない
    */
    bool IsError() {
      return isError;
    }
};

#endif

extern LoopCycleController loopCtrl;
