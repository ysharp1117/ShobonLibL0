/**
 * @file    SystickDriver.h
 * @author  \@y_sharp
 * @date    2015/12/28
 * @brief   Systickタイマを用いたタイマ割り込みのドライバです。
 * 全ての割り込み間隔の最大公約数をなるべく高く設定するようにして下さい(250以上を推奨)。
 * ShobonDeviceDriver.hを介してincludeされるので単体でincludeする必要はありません。
 */ 

#ifndef __SYSTICK_DRIVER__
#define __SYSTICK_DRIVER__

#ifdef __cplusplus
extern "C"{
#endif

/** @defgroup SystickDriver Systick割り込み関数
 * @par us単位でタイマ割り込みを発生させて登録された関数を実行します。
 * @par 内部タイマの割り込み間隔は登録した割り込み間隔の最大公約数です。割り込みによるオーバーヘッドを減らすために全ての割り込み間隔の最大公約数をなるべく大きく取るようにして下さい(250以上を推奨)。
 * @par 登録できるタイマの最大数はSYSTICK_IRQ_MAXで登録される数になります。
 */

#ifndef SYSTICK_IRQ_MAX
/** @ingroup SystickDriver
 * 割り込み登録数の最大数を表します。コンパイラのオプションで指定した場合はそちらを用います。
 */
#define SYSTICK_IRQ_MAX 8
#endif

///割り込み関数ポインタを表す型
typedef void (*SYSTICK_INTERRUPT_FUNC)();

extern int SystickAddInterrupt(int interval_us,SYSTICK_INTERRUPT_FUNC ptr);
extern void SystickRemoveInterrupt(int id);
extern void SystickStart(void);
extern void SystickStop(void);
extern uint64_t getNowTime_us(void);

#ifdef __cplusplus
}
#endif

#endif
