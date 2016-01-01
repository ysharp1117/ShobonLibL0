/**
 * @file    Delay.h
 * @author  \@y_sharp
 * @date    2016/01/01
 * @brief   TIMxを用いた単純時間待ちのドライバです。
 * ShobonDeviceDriver.hを介してincludeされるので単体でincludeする必要はありません。
 */ 

#ifndef __DELAY_TIM__
#define __DELAY_TIM__

/** @defgroup Delay 単純Delay関数
 * us,ms単位で単純Delayを行います。単一のタイマを用いるので時間待ち中には割り込みなどによって多重に呼び出すことは出来ません。複数箇所で使わないで下さい。
 */

extern void delay_ms(uint32_t time);
extern void delay_us(uint32_t time);

#define DELAY_TIM TIM2
#define DELAY_TIM_APB1 RCC_APB1ENR_TIM2EN
#define DELAY_TIM_APB2 0x00

#endif
