/**
 * @file    System.c
 * @author  \@y_sharp
 * @date    2015/12/28
 * @brief   他のドライバで共通利用する低水準な領域を扱うドライバです。
 */ 
#include "stm32l0xx.h"
#include "System.h"

/**
 * @ingroup System
 * @brief 現在のシステムクロックの値を返します。
 * @return システムクロック[Hz]
 */
uint32_t getSYSCLK(){
	SystemCoreClockUpdate();
	
	return SystemCoreClock;
}

/**
 * @ingroup System
 * @brief 現在のAHBバスのクロックを返します。
 * @return 現在のAHBバスのクロック[Hz]
 */
uint32_t getAHBCLK(){
	uint32_t presq;
	uint32_t temp = (RCC->CFGR&RCC_CFGR_HPRE)>>4&0xf;
	
	if(temp&0x08){
		unsigned int i;
		temp &= ~(0x08);
		presq = 2;
		for(i=0;i<temp;i++){
			presq*=2;
		}
	}else{
		presq = 1;
	}
	
	return getSYSCLK()/presq;
}

/**
 * @ingroup System
 * @brief 現在のAPB1バスのクロックを返します。
 * @return 現在のAPB1バスのクロック[Hz]
 */
uint32_t getAPB1CLK(){
	uint32_t presq;
	uint32_t temp = (RCC->CFGR&RCC_CFGR_PPRE1)>>8&0x7;
	
	if(temp&0x04){
		unsigned int i;
		temp &= ~(0x04);
		presq = 2;
		for(i=0;i<temp;i++){
			presq*=2;
		}
	}else{
		presq = 1;
	}
	
	return getAHBCLK()/presq;
}

/**
 * @ingroup System
 * @brief 現在のAPB2バスのクロックを返します。
 * @return 現在のAPB2バスのクロック[Hz]
 */
uint32_t getAPB2CLK(){
	uint32_t presq;
	uint32_t temp = (RCC->CFGR&RCC_CFGR_PPRE2)>>11&0x7;
	
	if(temp&0x04){
		unsigned int i;
		temp &= ~(0x04);
		presq = 2;
		for(i=0;i<temp;i++){
			presq*=2;
		}
	}else{
		presq = 1;
	}
	
	return getAHBCLK()/presq;
}

/**
 * @ingroup System
 * @brief タイマのクロックを返します。
 * @param TIMx クロックを取得するタイマペリフェラル
 * @return タイマのクロック[Hz]
 */
uint32_t getTIMxCLK(TIM_TypeDef *TIMx){
	uint32_t res=0;
	
	switch((uint32_t)TIMx){
	case TIM21_BASE:
	case TIM22_BASE:
		res = getAPB2CLK();
		break;
	case TIM2_BASE:
		res = getAPB1CLK();
		break;
	}
	return res;
}

/**
 * @ingroup System
 * @brief 割り込み優先度グループの設定を行います。4bitの優先度を2bitをグループ優先度，サブ優先度にそれぞれ割り当てています。SystemInit()関数で自動的に呼び出されます。
 * @return 戻り値はありません。
 */
void NVICGroupInit(){
	#define SUB_PRIO_CNT 2//(0~4)
	
	SCB->AIRCR = (0x5FA<<16) | ((SUB_PRIO_CNT+3)<<8);
}
