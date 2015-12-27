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
 * - \link System Systemドライバ \endlink - 他のドライバで共通利用する低水準な領域を扱うドライバです。
 * - \link SystickDriver 時間割り込みドライバ \endlink - 指定時間毎に指定した関数を呼び出す時間割り込みのライブラリです。Cortex-Mシリーズ共通のSystickタイマを用いています。
 */

/** @defgroup ShobonDeviceDriver
 * 各デバイスドライバのファイルをincludeします。
 */

#include "stm32l0xx.h"

#include "System.h"
#include "SystickDriver.h"

#ifdef __cplusplus
}
#endif

#endif
