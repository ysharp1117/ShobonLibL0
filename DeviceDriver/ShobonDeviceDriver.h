/**
 * @file    ShobonDeviceDriver.h
 * @author  \@y_sharp
 * @date    2015/12/28
 * @brief   DeviceDriverのヘッダを全てincludeします。
 */ 

#ifndef __SHOBON_DEVICE_DRIVER__
#define __SHOBON_DEVICE_DRIVER__

#ifdef __cplusplus
extern "C"
{
#endif
	
/**
 * @mainpage
 * ShobonLibL0のDeviceDriverはSTM32L0向けにマイコンの機能に対して容易にアクセスすることを目的として作成されたドライバです。
 *
 * 以下の内容から構成されます。利用する際はShobonDeviceDriver.hをincludeして下さい。
 *
 * - \link Delay 単純時間待ちドライバ \endlink - 指定した時間だけ処理を停止します。処理の停止中に発生した割り込み中などの条件では多重に使うことは出来ません。
 * - \link System Systemドライバ \endlink - 他のドライバで共通利用する低水準な領域を扱うドライバです。
 * - \link SystickDriver 時間割り込みドライバ \endlink - 指定時間毎に指定した関数を呼び出す時間割り込みのライブラリです。Cortex-Mシリーズ共通のSystickタイマを用いています。
 * - \link USARTDriver USARTドライバ \endlink - USARTドライバですが単一のUSARTのマルチバッファ通信しか実装していません。ユーザが自由に使うことは出来ません。
 */

/** @defgroup ShobonDeviceDriver
 * 各デバイスドライバのファイルをincludeします。
 */

#include "stm32l0xx.h"

#include "Delay.h"
#include "System.h"
#include "SystickDriver.h"
#include "USARTDriver.h"

#ifdef __cplusplus
}
#endif

#endif
