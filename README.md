ShobonLibL0
======================
STM32L0向けのShobonLibです。STM32L031のNucleo32を用いて動作確認を行っています。
大きく分けて以下の要素で構成されます。

+   Drivers/CMSIS

ST社/ARM社提供のマイコンのレジスタ定義。ARMマイコン共通ファイル。

+   Drivers/STM32L0xx\_HAL_Driver

ST社提供のマイコンチップを扱うためのドライバ。

+   Drivers/BSP

ST社提供のマイコンボード毎の定義ファイル。

+   DeviceDriver

マイコンの低水準な領域を扱うデバイスドライバ。

-----------------------
# 使い方
## 開発環境整備(最低限)
### [GCC ARM Embedded](https://launchpad.net/gcc-arm-embedded)(2016/09/27現在の最新版5.4-2016-q2への[直リン](https://launchpad.net/gcc-arm-embedded/5.0/5-2016-q2-update/+download/gcc-arm-none-eabi-5_4-2016q2-20160622-win32.exe))

ARM Cortex-Mシリーズコンパイラとして上記のサイトの物を用いる。インストール先はどこでもよい。PATHがむやみに長くなるのを防ぐためにC:\launcpad\とでもしておく。
インストールの最後に環境変数PATHに追加してくれる項目にチェックする。忘れたらC:\launchpad\binを環境変数に追加する(別のところに入れたら適宜読み替える)。

### 各種ツール群

基本的にねむいさん準拠です。
以下の説明の()内の項目についてはPATHを通して下さい。

+ [Coreutils](http://gnuwin32.sourceforge.net/packages/coreutils.htm)+[Make](http://gnuwin32.sourceforge.net/packages/make.htm)+[sed](http://gnuwin32.sourceforge.net/packages/sed.htm)+[Regex](http://gnuwin32.sourceforge.net/packages/regex.htm)(C:\Devz\Coreutils\bin):全てC:\Devz\Coreutils以下に解凍する。make,sed辺りのコマンドをPATHが通るところに配置する。
+ [Doxygen](http://www.stack.nl/~dimitri/doxygen/download.html#latestsrc)(C:\Devz\doxygen):doxygen.exeをPATHが通るところに配置する。
+ [OpenOCD](http://nemuisan.blog.bai.ne.jp/?eid=192848):openocd.exeをC:\Devz\ARM\OCDに配置するように解凍する。
+ [Insight](https://en.osdn.jp/projects/sfnet_netx-arm-elf-gc/releases/):arm-none-eabi-insight.exeをC:\Devz\ARM\insight\binに配置するように解凍する。

### [ST-Link Utility](http://www.st.com/content/st_com/ja/products/embedded-software/development-tool-software/stsw-link004.html)

標準設定でインストールを行う。
インストール後，ST-Link_CLI.exeがあるディレクトリにPATHを通す(標準ではC:\Program Files\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility辺り，64bit版とかは適宜読み替える)。

### [ST-Link v2-1ドライバ](http://www.st.com/content/st_com/ja/products/embedded-software/development-tool-software/stsw-link009.html)

マイコンへのプログラムの書き込みやデバッグ，シリアルポートを用いた通信にST-Link v2-1を用いている。これのドライバである。Windowsのビット数に対応するドライバをインストールする。

### [TeraTerm](http://www.forest.impress.co.jp/library/software/utf8teraterm/)

ShobonLibL0では標準出力はU(S)ARTにリターゲットされています。従って，PC-マイコン間をシリアル通信することでprintf()などの内容を受け取ることが出来ます。
標準のボーレートは115200です。Board/USARTSettings.defを編集することで変更することも出来ます。

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
