# Raspberry Pi Mouse Simulator's Tutorial for airy

[前回の記事 01_setup.md)](./01_setup.md) \
[前回の記事 00_Inventor.md](./00_Inventor.md)\
[前回の記事 02_kyori_sensor_test.md](./02_kyori_sensor_test.md)\

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
[解決]　ERROR: Service [/motor_on] is not available. -> raspi_ros_2の起動をする\
[参考][ROSのサービスとパラメータ](http://hara.jpn.com/_default/ja/Topics/ROS%E3%81%AE%E3%82%B5%E3%83%BC%E3%83%93%E3%82%B9%E3%81%A8%E3%83%91%E3%83%A9%E3%83%A1%E3%83%BC%E3%82%BF.html)
[参考][C++でシンプルなサービスとクライアントを書く](http://wiki.ros.org/ja/ROS/Tutorials/WritingServiceClient%28c%2B%2B%29)

## ROSサービスとは
* ノードが他のノードと通信するための方法
* Pub&Subと異なりノード通信は１対１の通信
* 同期・非同期の通信を行う

/motor_onのサービスが無いエラーが出たので調査する

### rosservice listコマンド
listコマンドで現在のROSサービスの一覧が見れる
```
$ rosservice list
/gazebo/apply_body_wrench
/gazebo/apply_joint_effort
...
/gazebo/spawn_sdf_model
/gazebo/spawn_urdf_model
...
/gazebo_ros_control/pid_gains/left_wheel_joint/set_parameters
/gazebo_ros_control/pid_gains/right_wheel_joint/set_parameters
/raspimouse_on_gazebo/controller_manager/list_controller_types
/raspimouse_on_gazebo/controller_manager/list_controllers
/raspimouse_on_gazebo/controller_manager/load_controller
/raspimouse_on_gazebo/controller_manager/reload_controller_libraries
/raspimouse_on_gazebo/controller_manager/switch_controller
/raspimouse_on_gazebo/controller_manager/unload_controller
/raspimouse_on_gazebo/controller_spawner/get_loggers
/raspimouse_on_gazebo/controller_spawner/set_logger_level
/raspimouse_on_gazebo/lightsensor_subscriber/get_loggers
/raspimouse_on_gazebo/lightsensor_subscriber/set_logger_level
/raspimouse_on_gazebo/robot_state_publisher/get_loggers
/raspimouse_on_gazebo/robot_state_publisher/set_logger_level
/raspimouse_on_gazebo/stepper_cmd_vel_publisher/get_loggers
/raspimouse_on_gazebo/stepper_cmd_vel_publisher/set_logger_level
/rosout/get_loggers
/rosout/set_logger_level
```
/motor_onのサービスが無いことがわかる
raspimouse_gazeboパッケージのraspimouse_with_samplemaze.launchを起動したあとに\
もう一度サービス一覧を見る
```
$ rosservice list
/buttons/get_loggers
/buttons/set_logger_level
/buzzer/get_loggers
/buzzer/set_logger_level
...
/gazebo/reset_simulation
/gazebo/reset_world
...
/gazebo_ros_control/pid_gains/left_wheel_joint/set_parameters
/gazebo_ros_control/pid_gains/right_wheel_joint/set_parameters
/leds/get_loggers
/leds/set_logger_level
/lightsensors/get_loggers
/lightsensors/set_logger_level
/motor_off
/motor_on
/motors/get_loggers
/motors/set_logger_level
/raspimouse_on_gazebo/controller_manager/list_controller_types
/raspimouse_on_gazebo/controller_manager/list_controllers
/raspimouse_on_gazebo/controller_manager/load_controller
/raspimouse_on_gazebo/controller_manager/reload_controller_libraries
/raspimouse_on_gazebo/controller_manager/switch_controller
/raspimouse_on_gazebo/controller_manager/unload_controller
/raspimouse_on_gazebo/controller_spawner/get_loggers
/raspimouse_on_gazebo/controller_spawner/set_logger_level
/raspimouse_on_gazebo/lightsensor_subscriber/get_loggers
/raspimouse_on_gazebo/lightsensor_subscriber/set_logger_level
/raspimouse_on_gazebo/robot_state_publisher/get_loggers
/raspimouse_on_gazebo/robot_state_publisher/set_logger_level
/raspimouse_on_gazebo/stepper_cmd_vel_publisher/get_loggers
/raspimouse_on_gazebo/stepper_cmd_vel_publisher/set_logger_level
/rosout/get_loggers
/rosout/set_logger_level
/timed_motion
```
今度は/motor_onのサービスが表示されたことから、/motor_onのサービスが無いエラーは\
raspimouse_with_samplemaze.launchを起動していなかったからだということがわかった

### rosservice type/info
typeコマンドでサービス型を調べる\
例として/raspimouse_on_gazebo/controller_spawner/get_loggersを上げる

```
$ rosservice type /raspimouse_on_gazebo/controller_spawner/get_loggers
roscpp/GetLoggers
```
infoコマンドで詳細を調べる
```
rosservice info /raspimouse_on_gazebo/controller_spawner/get_loggers
Node: /raspimouse_on_gazebo/controller_spawner
URI: rosrpc://192.168.12.114:42217
Type: roscpp/GetLoggers
Args: 
```
次に/motor_onのサービスを調べる
```
$ rosservice info /motor_on
Node: /motors
URI: rosrpc://192.168.12.114:46287
Type: std_srvs/Trigger
Args: 
```
上から、/motor_onのサービスはmotorsノードが実行していて\
リクエストにデータ無しで呼び出し(rosservice call /motor_on)\
レスポンスにstd_srvs/Trigger型で返すことがわかる。\
レスポンスの詳細を見るにはtypeコマンドの結果をrossrv showにパイプする\
```
$ rosservice type /motor_on | rossrv show
---
bool success
string message
```
レスポンスは２つ
1. bool型success::raspimouse_with_samplemaze.launch
2. string型のmessage::メッセージ(これは何)

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

