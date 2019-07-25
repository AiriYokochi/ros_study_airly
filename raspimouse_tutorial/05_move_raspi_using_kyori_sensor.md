# Raspberry Pi Mouse Simulator's Tutorial for airy

[前回の記事](./01_setup.md) 
[前回の記事](./00_Inventor.md)
[前回の記事](./02_kyori_sensor_test.md)

## 本日やったこと
1. [Part5_シミュレータ上のラズパイマウスを動かす方法](#Part5_シミュレータ上のラズパイマウスを動かす方法)

### 参考
* [Raspberry Pi Mouse Simulator's Tutorial](https://raspimouse-sim-tutorial.gitbook.io/project/)
* [how_to_install_simulator_xenial](https://github.com/rt-net/raspimouse_sim/wiki/how_to_install_simulator_xenial) 
* [作者ブログ](https://products.rt-net.jp/micromouse/archives/tag/ros)
### 時間
10:00-11:00::迷路の表示・距離センサの値の読み取り方\
13:00-14:00::モータの操作

### 次回やること
シミュレーションのチュートリアルを読む
* ROSサービスの調査

### 所感　時間がかかったこと、難しかった、やりたいことなど
[調べる] xacroからworldに変換する方法について、xacroのオプションについて\
[調べる] Warning [parser.cc:759] XML Attribute[xmlns:xacro] in element[sdf] not defined in SDF, ignoring. \
[解決]　ERROR: Service [/motor_on] is not available. -> raspi_ros_2の起動をする

## ROSサービスとは

## Part5_シミュレータ上のラズパイマウスを動かす方法
### 概要
距離センサの読み取り方のコマンド

### 実行方法
1. 迷路を表示
```
roslaunch raspimouse_gazebo raspimouse_with_samplemaze.launch
```
2. raspimouseの起動
```
roslaunch raspimouse_ros_2 raspimouse.launch
```
3. モータの通電
```
rosservice call /motor_on
```
4. left_hand.pyの起動
```
rosrun raspimouse_sim_tutorial left_hand.py
```


[トップへ](#本日やったこと)

<!--
```
プログラムを書く
```
-->

