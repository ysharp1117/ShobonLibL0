SHELL=cmd

ECHO = echo.exe

#プロジェクト名の設定
PROJ_NAME		= Template_l031

#ターゲットの定義
TARGET			= arm-none-eabi

#外部水晶の周波数を指定[Hz]
HSE_VALUE		= 8000000

#SWD書き込みの場合のCUIインタフェース名
ST-LINK_CLI = "ST-LINK_CLI.exe"

#SWD書き込みの場合のCUIインタフェースのオプション
ST-LINK_CLI_OPT = ID=0

#ライブラリフォルダの指定
#LIB_PATH = ../ShobonLibL0
LIB_PATH = ..

#SystickDriverの割り込み最大数
SYSTICKDRIVER_MAX = 8

############################
#メインのユーザファイル関係#
############################
#ユーザソースフォルダの指定
USER_SRC_DIRS := .
#ユーザヘッダファイルの位置(パスの末尾に/をつけない！)
USER_INCLUDE_DIRS = -I . \

###########
#CMSIS関係#
###########
#CMSISの基準位置
CMSIS_DIR = $(LIB_PATH)/Drivers/CMSIS
#CMSISのインクルードするファイルがあるフォルダの指定
CMSIS_INCLUDE_DIRS =	-I $(CMSIS_DIR)/Include \
						-I $(CMSIS_DIR)/Device/ST/STM32L0xx/Include \

#allを優先して実行するように設定
.PHONY: clean depend all commonlib
all: $(PROJ_NAME).hex

######################
#共通ライブラリの設定#
######################
COMMON_LIB_DIR = ../../common_l031

######################
#共通ライブラリの作成#
######################
include Makefiles/Makefile_dsplib
include Makefiles/Makefile_hallib
include Makefiles/Makefile_stm32l031k6_nucleo32

######################
#個別ライブラリの設定#
######################
include Makefiles/Makefile_devicedriver

###########
#リンカ関係#
###########
#リンカスクリプトの基準位置
LINKER_DIR = $(LIB_PATH)/linker
#リンカスクリプト名
LINKER_NAME = STM32L031.ld

############################
#スタートアップファイル関連#
############################
START_UP_DIR	:= .
START_UP_FILE	:= startup_stm32l031xx.s
START_UP_ASM	:= $(START_UP_DIR)/$(START_UP_FILE)

##################################
#以下は基本的には編集しないこと！#
##################################

#各種コマンド定義
TARGET_PREFIX	= $(TARGET)-
CC				= $(TARGET_PREFIX)gcc
CXX				= $(TARGET_PREFIX)g++
LD				= $(TARGET_PREFIX)g++
AS				= $(TARGET_PREFIX)as
AR				= $(TARGET_PREFIX)ar
OBJCOPY			= $(TARGET_PREFIX)objcopy
SIZE			= $(TARGET_PREFIX)size
RANLIB			= $(TARGET_PREFIX)ranlib
RM				= rm -rf

#ソースコードのリストを作成
USER_SRCS := $(wildcard ./*.c) $(wildcard ./*.cpp)

#各種コマンドのオプション

CFLAGS+=-c -g -mthumb -mcpu=cortex-m0plus -O0 -Wall -ffunction-sections -fdata-sections -mthumb -mlittle-endian
CFLAGS+=$(FPU_FLAG)

LINK_SCRIPT=$(LINKER_DIR)/$(LINKER_NAME)
LDFLAGS= -mthumb -mcpu=cortex-m0plus -lm
LDFLAGS+=-Wl,--gc-sections -T$(LINK_SCRIPT) -static  -Wl,-cref -Wl,-Map=$(PROJ_NAME).map -Wl,--defsym=malloc_getpagesize_P=0x1000 -lstdc++
LDFLAGS+= -L $(LINKER_DIR)

#各種定義
CFLAGS+=-D"USE_HAL_DRIVER"
CFLAGS+=$(USER_INCLUDE_DIRS)
CFLAGS+=$(CMSIS_INCLUDE_DIRS)
CFLAGS+=$(INCLUDE_DIRS)
CFLAGS+=-D"HSE_VALUE=$(HSE_VALUE)"
CFLAGS+=-D"HSE_STARTUP_TIMEOUT=((uint16_t)0x0500)"
CFLAGS+=-D"HSI_VALUE=16000000"
#DSPライブラリを使うために定義
CFLAGS+=-D"ARM_MATH_CM0PLUS"
#MCUの設定
CFLAGS+=-D"STM32L031xx"
#Systickドライバの割り込み最大数
CFLAGS+=-D"SYSTICK_IRQ_MAX=$(SYSTICKDRIVER_MAX)"

#C++向けのオプション設定
#CXXFLAGS= -fno-rtti -fno-exceptions -fnostartfiles $(CFLAGS)

#ユーザーオブジェクトファイル一覧
USER_OBJS := $(filter %.o,$(patsubst %.c,%.o,$(USER_SRCS))) $(filter %.o,$(patsubst %.cpp,%.o,$(USER_SRCS))) $(START_UP_ASM:%.s=%.o)

#全オブジェクトファイルにユーザオブジェクトを登録
OBJS+=$(USER_OBJS)

####################
#実際に処理する部分#
####################
$(PROJ_NAME).elf: $(OBJS) $(COMMON_LIB_DEPENDS)
	@$(ECHO) Generating elf file.
	$(LD) -o $@ $(OBJS) $(LDFLAGS) $(COMMON_LIBS)
	@$(ECHO)

$(PROJ_NAME).hex: $(PROJ_NAME).elf
	@$(ECHO) Generating hex file.
	$(OBJCOPY) -O ihex $^ $@
	$(SIZE) $@
	@$(ECHO)

clean:
	$(RM) $(OBJS) $(PROJ_NAME).elf $(PROJ_NAME).map $(PROJ_NAME).hex
	@$(ECHO)
	@$(ECHO) Clean up!

depend:
	@$(RM) Makefiles/Makefile.depend
	@$(ECHO) Generating Makefile.depend
	@$(CC) -MM -w $(CFLAGS) $(USER_SRCS) >>Makefiles/Makefile.depend
	@$(CC) -MM -w $(CFLAGS) $(DEVICE_DRIVER_SRCS) >>Makefiles/Makefile.depend
#	@$(CC) -MM -w $(CFLAGS) $(COMMON_LIB_SRCS) >>Makefiles/Makefile.depend

rom:
	@$(ECHO) Write binary to STM32 ROM(DFU)
	dfuw $(PROJ_NAME).hex

swd: $(PROJ_NAME).hex
	@$(ECHO) Write binary to STM32 ROM(SWD)
	@$(ST-LINK_CLI) -c SWD $(ST-LINK_CLI_OPT) -P $(PROJ_NAME).hex -V

#依存関係を取り込む
-include Makefiles/Makefile.depend

#共通ライブラリの一括コンパイル
commonlib:$(COMMON_LIBS)

#その他
.c.o:
	$(CC) $(CFLAGS) $< -o $@
#アセンブラコードも出力してみる
#	$(CC) $(CFLAGS) -S $<
	@$(ECHO)

.cpp.o:
	$(CXX) $(CFLAGS) $< -o $@
#アセンブラコードも出力してみる
#	$(CXX) $(CFLAGS) -S $<
	@$(ECHO)

.s.o:
	$(CC) $(CFLAGS) $< -o $@
	@$(ECHO)
