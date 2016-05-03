######################
#プロジェクト毎の設定#
######################
#プロジェクト名の設定
PROJ_NAME = Template_l031_nucleo

#printfの出力先の設定
PRINTF_OUT = PRINTF_OUT_UART

#SystickDriverの割り込み最大数
SYSTICKDRIVER_MAX = 8

##############################################
#Makefileでのデフォルトで実行されるものを設定#
##############################################
#allを優先して実行するように設定
.PHONY: clean depend all commonlib
all: $(PROJ_NAME).bin $(PROJ_NAME).hex

##############################
#プロジェクト固有の独自ルール#
##############################
#独自ルールはこの辺りに書く

##############################
#以下は外部Makefileの読み込み#
##############################
include Makefile_Settings
include $(BOARD_SETTINGS_PATH)/Makefile_core
