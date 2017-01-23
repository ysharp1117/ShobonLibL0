/**
 * @file    Delay.h
 * @author  \@y_sharp
 * @date    2017/01/23
 * @brief   TIMxを用いた単純時間待ちのドライバです。
 * ShobonDeviceDriver.hを介してincludeされるので単体でincludeする必要はありません。
 */ 

#ifndef __SHOBON_DELAY_TIM__
#define __SHOBON_DELAY_TIM__

#ifdef __cplusplus
extern "C"{
#endif

/** @defgroup Delay 単純Delay関数
 * us,ms単位で単純Delayを行います。単一のタイマを用いるので時間待ち中には割り込みなどによって多重に呼び出すことは出来ません。複数箇所で使わないで下さい。
 */

extern void delay_ms(uint32_t time);
extern void delay_us(uint32_t time);

#ifdef __cplusplus
}
#endif

#endif
