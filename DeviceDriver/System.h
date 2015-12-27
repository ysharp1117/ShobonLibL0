/**
 * @file    System.h
 * @author  \@y_sharp
 * @date    2015/12/28
 * @brief   他のドライバで共通利用する低水準な領域を扱うドライバです。
 * ShobonDeviceDriver.hを介してincludeされるので単体でincludeする必要はありません。
 */ 

#ifndef __SHOBON_SYSTEM__
#define __SHOBON_SYSTEM__

#ifdef __cplusplus
extern "C"
{
#endif
	
/** @defgroup System システムドライバ
 * クロックの取得やデバッガピンの設定など低水準な領域を扱うドライバです。
 */

extern uint32_t getSYSCLK(void);
extern uint32_t getAHBCLK(void);
extern uint32_t getAPB1CLK(void);
extern uint32_t getAPB2CLK(void);
extern uint32_t getTIMxCLK(TIM_TypeDef *TIMx);

extern void NVICGroupInit(void);

#ifdef __cplusplus
}
#endif

#endif
