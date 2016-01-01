/**
 * @file    Delay.c
 * @author  \@y_sharp
 * @date    2016/01/01
 * @brief   TIMxを用いた単純時間待ちのドライバです。
 */ 

#include "stm32l0xx.h"
#include "System.h"
#include "Delay.h"

/**
 * @ingroup Delay
 * @brief [ms]単位で単純Delayを行います。
 * @param time [ms]単位のDelay時間
 * @return 戻り値はありません。
 */
void delay_ms(uint32_t time){
	int64_t _time = time;
	int64_t qt_time;
	int tim_clockm = getTIMxCLK(DELAY_TIM)/1000000;
	
	//DELAY_TIMを有効化
	RCC->APB1ENR |= DELAY_TIM_APB1;
	RCC->APB2ENR |= DELAY_TIM_APB2;
	DELAY_TIM->PSC = tim_clockm*1000/4 -1;//0.25ms毎にカウント
	DELAY_TIM->EGR |= TIM_EGR_UG;//プリスケーラ比変更したので更新イベントを発生
	while(_time>0){
		
		qt_time = 4*_time-1;
		
		if(qt_time>65535)qt_time = 65535;
		
		DELAY_TIM->ARR = qt_time;//時間を代入
		
		DELAY_TIM->CR1 &= ~(TIM_CR1_CEN|TIM_CR1_OPM);
		DELAY_TIM->CNT = 0x00;
		DELAY_TIM->CR1 |= TIM_CR1_CEN|TIM_CR1_OPM;
		while(DELAY_TIM->CR1&TIM_CR1_CEN);
		
		_time -= 16384;
		
	}
	
	//DELAY_TIMを無効化
	RCC->APB1ENR &= ~DELAY_TIM_APB1;
	RCC->APB2ENR &= ~DELAY_TIM_APB2;
	
}

/**
 * @ingroup Delay
 * @brief [us]単位で単純Delayを行います。
 * @param time [us]単位のDelay時間
 * @return 戻り値はありません。
 */
void delay_us(uint32_t time){
	int64_t _time = time;
	int64_t qt_time;
	int tim_clockm = getTIMxCLK(DELAY_TIM)/1000000;
	
	//DELAY_TIMを有効化
	RCC->APB1ENR |= DELAY_TIM_APB1;
	RCC->APB2ENR |= DELAY_TIM_APB2;
	DELAY_TIM->PSC = tim_clockm/4 -1;//0.25us毎にカウント
	DELAY_TIM->EGR |= TIM_EGR_UG;//プリスケーラ比変更したので更新イベントを発生
	while(_time>0){
		
		qt_time = 4*_time-1;
		
		if(qt_time>65535)qt_time = 65535;
		
		DELAY_TIM->ARR = qt_time;//時間を代入
		
		DELAY_TIM->CR1 &= ~(TIM_CR1_CEN|TIM_CR1_OPM);
		DELAY_TIM->CNT = 0x00;
		DELAY_TIM->CR1 |= TIM_CR1_CEN|TIM_CR1_OPM;
		while(DELAY_TIM->CR1&TIM_CR1_CEN);
		
		_time -= 16384;
		
	}
	
	//DELAY_TIMを無効化
	RCC->APB1ENR &= ~DELAY_TIM_APB1;
	RCC->APB2ENR &= ~DELAY_TIM_APB2;
	
}
