ShobonLibL0
======================
STM32L0向けのShobonLibです。STM32L031のNucleo32を用いて動作確認を行っています。
大きく分けて以下の要素で構成されます。

+   Drivers/CMSIS

ST社/ARM社提供のマイコンのレジスタ定義。ARMマイコン共通ファイル。

+   Drivers/STM32L0xx\_HAL_Driver

ST社提供のマイコンチップを扱うためのドライバ

+   Drivers/BSP

ST社提供のマイコンボード毎の定義ファイル。

+   DeviceDriver

@y_sharpが作ったマイコンの低水準な領域を扱うデバイスドライバ。

-----------------------
# 使い方
## 開発環境整備(最低限)
### [GCC ARM Embedded](https://launchpad.net/gcc-arm-embedded)(2015/12/28現在の最新版5.2-2015q4への[直リン](https://launchpad.net/gcc-arm-embedded/5.0/5-2015-q4-major/+download/gcc-arm-none-eabi-5_2-2015q4-20151219-win32.exe))

ARM Cortex-Mシリーズコンパイラとして上記のサイトの物を用いる。環境変数があまり長くなってもあれなのでインストール先はC:\launcpad\とでもしておく。
インストールの最後に環境変数PATHに追加してくれる項目にチェックする。忘れたらC:\launchpad\binを環境変数に追加する。

### 各種ツール群

基本的にねむいさん準拠です。
以下の説明の()内の項目についてはPATHを通して下さい。

+ Coreutils+GNU Make(C:\Devz\Coreutils\bin)

### ST-Link Utility(2015/12/28現在の最新版はv3.8.0)

標準設定でインストールを行う。
インストール後，ST-Link_CLI.exeがあるディレクトリにPATHを通す(標準ではC:\Program Files\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility辺り)。

### テキストエディタ

メモ帳以外のテキストエディタを使って下さい。おすすめは[サクラエディタ](http://sakura-editor.sourceforge.net/)です。

## 新規プログラム作成

1. Template_l031フォルダをShobonLibL0フォルダと同じ階層にコピーする。
2. プログラムフォルダ名を変更する。
3. Makefileをテキストエディタで開き，PROJ_NAMEの行を書き換えてプロジェクト名を変更する。
4. プログラムフォルダでコマンドプロンプトを開く。
5. make commonlib
6. make depend
7. make
8. main.cppを編集し快適？な実機実験へ。includeするファイルを変更した場合は6. から，そうでなければ7. を実行すればコンパイルされる。

## 書き込みとデバッグ
### 書き込み
書き込みの際にはマイコンとUSBケーブルで接続して，デバッガ(ST-Link v2-1(from ST Nucleo))を利用する。
電源を投入し以下のコマンドを入力する。

    make swd   #ST-Link Utility利用 書き込みの進捗がわかる
