/**
 * @file    USARTDriver.h
 * @author  \@y_sharp
 * @date    2016/01/01
 * @brief   USARTを用いたマルチバッファ送信のドライバです。
 * syscalls.c 内で標準出力の出力先として利用されています。ユーザが利用する必要はありません。
 * ShobonDeviceDriver.hを介してincludeされるので単体でincludeする必要はありません。
 */ 

#ifndef __SHOBON_USART_DRIVER__
#define __SHOBON_USART_DRIVER__

/** @defgroup USARTDriver USARTドライバ
 * @par
 * USARTドライバですが現在単一のUSARTのマルチバッファ通信のみ実装しています。
 * @par
 * ユーザが自由に利用することは出来ません。初期化はSystemInit()で呼び出されているので不要です。
 */

extern void USART_Init();
extern int send_str_DMA(char str[],uint16_t size);
extern uint8_t getUARTBuffer(void);

#endif
