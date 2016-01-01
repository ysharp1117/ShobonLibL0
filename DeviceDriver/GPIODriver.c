/**
 * @file    GPIODriver.c
 * @author  \@y_sharp
 * @date    2016/01/01
 * @brief   GPIOペリフェラルへのアクセスを行います。
 */ 

#include "stm32l0xx.h"
#include "GPIODriver.h"

GPIO_TypeDef* gpio_base_table[PORT_NUM]= {
	GPIOA,
	GPIOB,
	GPIOC,
#ifdef GPIOD
	GPIOD,
#endif
#ifdef GPIOE
	GPIOE,
#endif
#ifdef GPIOH
	GPIOH,
#endif
};

//GPIOクロックテーブル
const uint32_t gpio_clock_table[PORT_NUM]={
	RCC_IOPENR_GPIOAEN,
	RCC_IOPENR_GPIOBEN,
	RCC_IOPENR_GPIOCEN,
#ifdef GPIOD
	RCC_IOPENR_GPIODEN,
#endif
#ifdef GPIOE
	RCC_IOPENR_GPIOEEN,
#endif
#ifdef GPIOH
	RCC_IOPENR_GPIOHEN,
#endif
};

void GPIO_SetInput(GPIO_TypeDef* gpio,int pin){
	uint32_t temp;
	int i=0;
	int index;
	
	while(pin>>i){
		if((pin>>i)&0x01){
			index=i;
			//格納
			temp = gpio->MODER;
			
			temp &= ~(0x3<<(index*2));
			
			//セット
			gpio->MODER = temp;
		}
		i++;
	}
	
}

void GPIO_SetOutput(GPIO_TypeDef* gpio,int pin){
	uint32_t temp;
	int i=0;
	int index;
	
	while(pin>>i){
		if((pin>>i)&0x01){
			index=i;
			//格納
			temp = gpio->MODER;
			
			temp &= ~(0x3<<(index*2));
			temp |= 0x1<<(index*2);
			
			//セット
			gpio->MODER = temp;
		}
		i++;
	}
	
}

void GPIO_SetAFMode(GPIO_TypeDef* gpio,int pin){
	uint32_t temp;
	int i=0;
	int index;
	
	while(pin>>i){
		if((pin>>i)&0x01){
			index=i;
			//格納
			temp = gpio->MODER;
			
			temp &= ~(0x3<<(index*2));
			temp |= 0x2<<(index*2);
			
			//セット
			gpio->MODER = temp;
		}
		i++;
	}
	
}

void GPIO_SetAnalogMode(GPIO_TypeDef* gpio,int pin){
	uint32_t temp;
	int i=0;
	int index;
	
	while(pin>>i){
		if((pin>>i)&0x01){
			index=i;
			//格納
			temp = gpio->MODER;
			
			temp &= ~(0x3<<(index*2));
			temp |= 0x3<<(index*2);
			
			//セット
			gpio->MODER = temp;
		}
		i++;
	}
	
}

void GPIO_SetPullUp(GPIO_TypeDef* gpio,int pin){
	uint32_t temp;
	int i=0;
	int index;
	
	while(pin>>i){
		if((pin>>i)&0x01){
			index=i;
			//格納
			temp = gpio->PUPDR;
			
			temp &= ~(0x3<<(index*2));
			temp |= 0x1<<(index*2);
			
			//セット
			gpio->PUPDR = temp;
		}
		i++;
	}
	
}

void GPIO_SetPullDown(GPIO_TypeDef* gpio,int pin){
	uint32_t temp;
	int i=0;
	int index;
	
	while(pin>>i){
		if((pin>>i)&0x01){
			index=i;
			//格納
			temp = gpio->PUPDR;
			
			temp &= ~(0x3<<(index*2));
			temp |= 0x2<<(index*2);
			
			//セット
			gpio->PUPDR = temp;
		}
		i++;
	}
	
}

void GPIO_ResetPullUpDown(GPIO_TypeDef* gpio,int pin){
	uint32_t temp;
	int i=0;
	int index;
	
	while(pin>>i){
		if((pin>>i)&0x01){
			index=i;
			//格納
			temp = gpio->PUPDR;
			
			temp &= ~(0x3<<(index*2));
			
			//セット
			gpio->PUPDR = temp;
		}
		i++;
	}
	
}

void GPIO_SetAFR(GPIO_TypeDef* gpio,int pin,uint8_t afr){
	uint32_t temp;
	int i=0;
	int index;
	
	while(pin>>i){
		if((pin>>i)&0x01){
			index=i;
			//格納
			temp = gpio->AFR[index/8];
			
			temp &= ~(0xf<<((index%8)*4));
			temp |= (afr<<((index%8)*4));//AFを設定
			
			//セット
			gpio->AFR[index/8] = temp;
		}
		i++;
	}
	
}
