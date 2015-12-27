#include <stdio.h>
#include "stm32l0xx.h"
#include "stm32l0xx_hal.h"
#include "stm32l0xx_nucleo_32.h"
#include "ShobonDeviceDriver.h"
#include "main.h"

void led_tick(){
	BSP_LED_Toggle(LED_GREEN);
}

int main(void){
	HAL_Init();
	
	BSP_LED_Init(LED_GREEN);
	
	SystickStop();
	SystickAddInterrupt(500000,led_tick);//500000us=500ms=0.5s
	SystickStart();
	
	for(;;){
		//その他の処理
	}
}
