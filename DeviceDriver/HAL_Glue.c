/**
 * @file    HAL_Glue.c
 * @author  \@y_sharp
 * @date    2016/01/09
 * @brief   HALドライバをShobonLibで利用するためのグルー関数群です。
 */ 
#include "stm32l0xx_hal.h"
#include "SystickDriver.h"

/**
 * @ingroup HAL_Glue
 * @brief HAL_Init 時のSystick初期化をSystickDriver対応に書き換えます。
 * @return 常に HAL_OK
 */
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority){
	SystickAddInterrupt(1000,HAL_IncTick);//1ms毎にHALシステムタイマカウンタをインクリメント
	SystickStart();
	return HAL_OK;
}
