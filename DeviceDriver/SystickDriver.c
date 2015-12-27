/**
 * @file    SystickDriver.c
 * @author  \@y_sharp
 * @date    2015/12/28
 * @brief   Systickタイマを用いたタイマ割り込みのドライバです。
 * 全ての割り込み間隔の最大公約数をなるべく高く設定するようにして下さい(250以上を推奨)。
 */ 

#include "stm32l0xx.h"//CMSIS準拠の他のマイコンに移植の際はこの一行のみ変更すれば利用可能になります。
#include "SystickDriver.h"

#ifndef NULL
#define NULL ((void*)0)
#endif

//割り込み管理構造体の定義
typedef struct{
	int id;
	void (*ptr)();
	int interval_us;
	struct SYSTICK_ARRAY* prev_ptr;
	struct SYSTICK_ARRAY* next_ptr;
}SYSTICK_ARRAY;

//内部変数の定義
static SYSTICK_ARRAY array[SYSTICK_IRQ_MAX]={{0}};
static SYSTICK_ARRAY array_base={0};
static int next_id = 1;//割り込み番号の次の値
static int timer_cyc = 1;//タイマ割り込み間隔[us]
static int cnt = 0;//割り込み登録数
static uint64_t time=0;//タイマスタート時からの積算時間。全ての割り込みの最小公倍数の時にリセット
static uint64_t past_time=0;//全ての割り込みの最小公倍数の時にそれまでの積算時間を加算

//最大公約数を返します。
static int calcEuclid(int a,int b){
	int c;
	while((c=a%b)){
		a=b;
		b=c;
	}
	return b;
}

/**
 * @ingroup SystickDriver
 * @brief タイマ割り込みを登録します。 SystickStart 後に追加で登録する場合は必ずSystickStop してから登録して再度 SystickStart して下さい。
 * @param interval_us タイマ割り込みの間隔を指定します。
 * @param ptr タイマ割り込みの関数を指定します。
 * @return 割り込み登録番号を返します。 SystickRemoveInterrupt の際に必要となる番号です。
 * @see SystickRemoveInterrupt, SystickStart, SystickStop
 */
int SystickAddInterrupt(int interval_us,void (*ptr)()){
	int pos;
	int max=1;
	static int last_pos=0;
	
	//利用されていない配列の探索
	for(pos=0;pos<SYSTICK_IRQ_MAX;pos++){
		if(array[pos].id==0){
			max=0;
			break;
		}
	}
	
	//全て利用中の場合は0を返す
	if(max!=0)return 0;
	
	//配列に情報を格納
	array[pos].id = next_id;
	array[pos].ptr = ptr;
	array[pos].interval_us = interval_us;
	
	//前後の割り込みポインタをつなぐ
	if(next_id==1){
		array_base.next_ptr = (struct SYSTICK_ARRAY*)&array[pos];
	}else{
		array[last_pos].next_ptr = (struct SYSTICK_ARRAY*)&array[pos];
		array[pos].prev_ptr = (struct SYSTICK_ARRAY*)&array[last_pos];
		
	}
	last_pos = pos;
	
	//IDをインクリメント
	next_id++;
	cnt++;
	
	return array[pos].id;
}

/**
 * @ingroup SystickDriver
 * @brief 登録したタイマ割り込みを取り消します。SystickStart 後に取り消す場合は一旦SystickStop してからSystickStart することを推奨します。
 * @param id SystickAddInterrupt の戻り値である割り込み登録番号を指定します。
 * @return 戻り値はありません。
 * @see SystickAddInterrupt, SystickStart, SystickStop
 */
void SystickRemoveInterrupt(int id){
	int pos;
	
	//該当の配列の探索とID情報の削除
	for(pos=0;pos<SYSTICK_IRQ_MAX;pos++){
		if(array[pos].id==id){
			array[pos].id=0;
			((SYSTICK_ARRAY*)array[pos].prev_ptr)->next_ptr = array[pos].next_ptr;
			((SYSTICK_ARRAY*)array[pos].next_ptr)->prev_ptr = array[pos].prev_ptr;
			cnt--;
			break;
		}
	}
	
	//全て利用されていなければID情報を初期化して割り込みを停止
	if(cnt==0){
		next_id=1;
		SystickStop();
	}
}

/**
 * @ingroup SystickDriver
 * @brief SystickDriverによるタイマ割り込みを開始します。
 * @return 戻り値はありません。
 * @see SystickStop
 */
void SystickStart(){
	int interval;
	SYSTICK_ARRAY* now_ptr = &array_base;
	
	//割り込みが登録されていないならば開始しない
	if(now_ptr->next_ptr==NULL)return;
	
	//割り込み間隔の決定
	interval = now_ptr->interval_us;
	while((now_ptr=(SYSTICK_ARRAY*)now_ptr->next_ptr)){
		interval = calcEuclid(interval,now_ptr->interval_us);
	}
	
	//割り込み設定
	timer_cyc = interval;
	
	//システムクロックの取得
	SystemCoreClockUpdate();
	
	//割り込みの設定
	SysTick_Config(SystemCoreClock/1000000*timer_cyc);
}

/**
 * @ingroup SystickDriver
 * @brief SystickDriverによるタイマ割り込みを無効にします。
 * @return 戻り値はありません。
 * @see SystickStart
 */
void SystickStop(){
	SysTick->CTRL&=~(SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk);
}

/**
 * @ingroup SystickDriver
 * @brief Systickタイマのアンダーフロー割り込みです。直接呼び出さないで下さい。
 * @return 戻り値はありません。
 */
void SysTick_Handler(){
	SYSTICK_ARRAY* now_ptr = &array_base;
	int now_cnt=0;
	
	//登録順に実行
	while((now_ptr=(SYSTICK_ARRAY*)now_ptr->next_ptr)){
		if(!(time%now_ptr->interval_us)){
			(*(now_ptr->ptr))();
			now_cnt++;
		}
	}
	
	//登録された割り込みの最小公倍数の時はカウンタをリセット
	if(now_cnt==cnt){
		past_time+=time;
		time=0;
	}
	
	time+=timer_cyc;
}

/**
 * @ingroup SystickDriver
 * @brief SystickStart を呼び出してからの経過時間を単位[us]で返します。
 * @return 経過時間[us]。
 */
uint64_t getNowTime_us(){
	return time+past_time;
}
