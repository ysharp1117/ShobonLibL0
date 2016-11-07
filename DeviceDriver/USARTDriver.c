/**
 * @file    USARTDriver.c
 * @author  \@y_sharp
 * @date    2016/11/08
 * @brief   USARTを用いたマルチバッファ送信のドライバです。
 * syscalls.c 内で標準出力の出力先として利用されています。ユーザが利用する必要はありません。
 */ 

#include "stm32l0xx.h"
#include "System.h"
#include "GPIODriver.h"
#include "USARTDriver.h"
#include "Board/USARTSettings.def"

volatile static uint8_t trns_buf[TRNS_CNT][TRNS_BUF]={{0}};
volatile static uint16_t trns_cnt[TRNS_CNT]={0};
static uint16_t trns_data_cnt=0;
static uint16_t trns_proc_now=0;

//GPIOテーブル→GPIOポート参照マクロ
#define GPIO(port) ((GPIO_TypeDef*) gpio_base_table[port])

//USARTテーブル
static const uint32_t usart_list[]={
	0x00,//dummy
	USART2_BASE,
};

//USARTクロックテーブル(APB1)
static const uint32_t usart_clock_apb1[]={
	0x00,//dummy
	RCC_APB1ENR_USART2EN,
};

//USARTクロックテーブル(APB2)
static const uint32_t usart_clock_apb2[]={
	0,
	0,
};

static uint8_t getAFR(USART_TypeDef* usart){
	if(usart==USART2){
		return 0x04;
	}
	
	return 0x00;
}

static void setBaudRate(USART_TypeDef* usart,float baudrate){
	uint32_t usart_clk;
	
	if((usart==USART2)){
		usart_clk = getAPB1CLK();
	}
	
	usart->BRR = usart_clk/(8*(2-(usart->CR1&USART_CR1_OVER8?1:0))*baudrate)*16;
	if(usart->CR1&USART_CR1_OVER8){
		usart->BRR = ((usart->BRR&0xfff8)<<1)|(usart->BRR&0x7);
	}
}

static void setTransmitStatus(int state){
	if(state){
		//DMA Streamを有効化
		USARTx_TX_DMA_Channel->CCR |= DMA_CCR_EN;
	}else{
		//DMA Streamを無効化
		USARTx_TX_DMA_Channel->CCR &= ~DMA_CCR_EN;
	}
}

static void setTransmitData(uint32_t* addr,int size){
	while(USARTx_TX_DMA_Channel->CCR&DMA_CCR_EN);//Disableの確認
	USARTx_TX_DMA_Channel->CMAR = (uint32_t)addr;
	USARTx_TX_DMA_Channel->CNDTR = size;
	setTransmitStatus(1);
}

static void DMA_USARTx_TX_Init(){
	uint32_t temp;
	USART_TypeDef* USARTx = (USART_TypeDef*)usart_list[USART-1];

	//割り込みの登録
	//優先度4bitをグループ優先度2bit,サブ優先度2bitに設定
	NVICGroupInit();
	//割り込み設定(グループ優先度1に設定)
	NVIC_SetPriority(USARTx_TX_DMA_Channel_IRQn,(1<<2)|(0<<0));
	NVIC_EnableIRQ(USARTx_TX_DMA_Channel_IRQn);
	
	//DMAにクロック供給
	if(USARTx_TX_DMA==DMA1){
		RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	}
	
	//ペリフェラルのアドレスを代入
	USARTx_TX_DMA_Channel->CPAR = (uint32_t)&USARTx->TDR;
	
	//メモリのアドレスを代入
	USARTx_TX_DMA_Channel->CMAR = (uint32_t)&trns_buf;
	
	//CRレジスタの設定
	temp = 0x00;//初期化
	//メモリインクリメントモードに設定
	temp |= DMA_CCR_MINC;
	
	//ペリフェラルへの書き込みに設定
	temp |= DMA_CCR_DIR;
	
	//転送完了割り込みを有効
	temp |= DMA_CCR_TCIE;
	
	//ペリフェラルからのデータとメモリのサイズを指定
	temp &= ~(DMA_CCR_MSIZE|DMA_CCR_PSIZE);
	temp |= 0x00|0x00;//双方共に8bitに指定
	
	//DMAのチャネルを選択
	if(USARTx_TX_DMA==DMA1){
		DMA1_CSELR->CSELR &= ~(0xf<<((USARTx_TX_DMA_Ch-1)*4));
		DMA1_CSELR->CSELR |= USARTx_TX_DMA_CSELR<<((USARTx_TX_DMA_Ch-1)*4);
	}
	
	//CRレジスタの設定を反映
	USARTx_TX_DMA_Channel->CCR = temp;
	
}

static uint8_t initialized = 0;

void USART_Init(){
	USART_TypeDef* USARTx = (USART_TypeDef*)usart_list[USART-1];
	
	if(initialized)return;
	initialized=1;
	
	//GPIOにクロックを供給
	RCC->IOPENR |= gpio_clock_table[TX_PORT] | gpio_clock_table[RX_PORT];
	//利用するUSARTにクロックを供給
	RCC->APB1ENR |= usart_clock_apb1[USART-1];
	RCC->APB2ENR |= usart_clock_apb2[USART-1];
	
	//TXピンの設定
	GPIO_SetAFMode(GPIO(TX_PORT),1<<TX_PIN);//AF Modeに設定
	GPIO(TX_PORT)->OTYPER &= ~(0x1<<TX_PIN);//プッシュプルに設定
	GPIO(TX_PORT)->OSPEEDR |= 0x3<<(TX_PIN*2);//最大50MHz出力に設定
	GPIO_ResetPullUpDown(GPIO(TX_PORT),1<<TX_PIN);//プルダウン、プルアップ無し
	GPIO_SetAFR(GPIO(TX_PORT),1<<TX_PIN,getAFR(USARTx));
	
	//RXピンの設定
	GPIO_SetAFMode(GPIO(RX_PORT),1<<RX_PIN);//AF Modeに設定
	GPIO_ResetPullUpDown(GPIO(RX_PORT),1<<RX_PIN);//プルダウン、プルアップ無し
	GPIO_SetAFR(GPIO(RX_PORT),1<<RX_PIN,getAFR(USARTx));
	
	//DMAの初期化
	DMA_USARTx_TX_Init();
	
	//USARTを有効化
	USARTx->CR1 |= USART_CR1_UE;
	
	//データ長は8bit
	USARTx->CR1 &= ~USART_CR1_M;
	
	//ストップビットは1
	USARTx->CR2 &= ~USART_CR2_STOP;
	
	//トランスミッタはDMAを用いる
	USARTx->CR3 |= USART_CR3_DMAT;
	
	//ボーレートの設定(クロックから自動計算)
	setBaudRate(USARTx,BaudRate);
	
	//トランスミッタの有効化
	USARTx->CR1 |= USART_CR1_TE;
	
	//受信割り込み有効
	USARTx->CR1 |= USART_CR1_RXNEIE;
	NVIC_EnableIRQ(USARTx_IRQn);
	
	//レシーバの有効化
	USARTx->CR1 |= USART_CR1_RE;
	
	//DMA受信の有効化
	//USARTx->CR3 |= USART_CR3_DMAR;
}

int send_str_DMA(char str[],size_t size){
	int i;
	
	__disable_irq();
	
	//キューが満タンの場合はエラーを返す
	if(trns_data_cnt>=TRNS_CNT){
		i=-1;
	}else{
		//バッファサイズを超える場合はバッファサイズまで格納
		if(size>TRNS_BUF)size=TRNS_BUF;
		//DMAの最大バッファサイズは65535(2^16-1)なので飽和演算の処理
		if(size>65535)size=65535;
		
		for(i=0;i<size;i++){
			trns_buf[trns_proc_now][i] = *str++;
		}
		
		//送信バイト数を格納
		trns_cnt[trns_proc_now] = i;
		
		//キューにタスクがない場合は直ちにDMA送信
		trns_data_cnt++;//キューの個数をインクリメント
		if(trns_data_cnt==1 && !(USARTx_TX_DMA_Channel->CCR&DMA_CCR_EN)){
			setTransmitData((uint32_t*)trns_buf[trns_proc_now],trns_cnt[trns_proc_now]);
		}
		
		//バッファ管理カウンタをインクリメント
		trns_proc_now++;
		trns_proc_now %= TRNS_CNT;
	}
	
	__enable_irq();
	
	return i;
}

void USARTx_TX_DMA_Channel_IRQ_Handler(){
	
	__disable_irq();
	
	//DMA Streamを無効化
	setTransmitStatus(0);
	//DMA送信終了したので割り込みフラグを消してDMAを無効にする
	USARTx_TX_DMA->IFCR = DMA_IFCR_CTCIF1<<((USARTx_TX_DMA_Ch-1)*4);
	
	//カウンタを1減らす
	trns_data_cnt--;
	
	//キューに残っているタスクがある場合は続いて送信
	if(trns_data_cnt>0){
		setTransmitData((uint32_t*)trns_buf[(trns_proc_now-trns_data_cnt+TRNS_CNT)%TRNS_CNT],trns_cnt[(trns_proc_now-trns_data_cnt+TRNS_CNT)%TRNS_CNT]);
	}
	
	__enable_irq();
}

#define RX_BUF_SIZE 32
static char usart_rx_buf[RX_BUF_SIZE];
static int rx_in_ptr=0;
static int rx_out_ptr=0;

uint8_t getUARTBuffer(){
	uint8_t res;
	while(rx_in_ptr==rx_out_ptr);
	res = usart_rx_buf[rx_out_ptr];
	rx_out_ptr = (rx_out_ptr+1)%RX_BUF_SIZE;
	return res;
}

void USARTx_IRQ_Handler(){
	USART_TypeDef* USARTx = (USART_TypeDef*)usart_list[USART-1];
	
	__disable_irq();
	
	if(USARTx->ISR&USART_ISR_RXNE){
		usart_rx_buf[rx_in_ptr] = USARTx->RDR;
		rx_in_ptr = (rx_in_ptr+1)%RX_BUF_SIZE;
	}
	
	__enable_irq();
}
