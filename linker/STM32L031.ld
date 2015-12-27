/* Linker Script For STM32L031 (NUCLEO-L031K6) 			*/
/* 20140303 Nemui Trinomius Based on CMSIS Linker Scripts 	*/ 
/* 20151228 @y_sharp ねむいさんのSTM32F030向けのリンカより改変(元ファイル名STM32F030R8T6.ld) */

OUTPUT_FORMAT ("elf32-littlearm")

/* Memory Spaces Definitions */
MEMORY
{
	RAM1 (xrw)	: ORIGIN = 0x20000000, LENGTH = 8k		/* Main Embedded SRAM 					*/
	ROM (rx)	: ORIGIN = 0x08000000, LENGTH = 32K		/* Main Embedded FlashROM				*/
}

/* higher address of the stack bottom */
_estack = ORIGIN(RAM1)+LENGTH(RAM1);

/* higher address of the heap end */
/*_heap_end = ORIGIN(RAM1)+LENGTH(RAM1)-4;*/	/* due to 4byte alignments */


/* include the section management sub-script */
INCLUDE "STM32F0xx_FLASH.ld"
