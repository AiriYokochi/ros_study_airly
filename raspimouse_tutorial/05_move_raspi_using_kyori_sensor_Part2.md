# Raspberry Pi Mouse Simulator's Tutorial for airy

[前回の記事 01_setup.md)](./01_setup.md)\
[前回の記事 00_Inventor.md](./00_Inventor.md)\
[前回の記事 02_kyori_sensor_test.md](./02_kyori_sensor_test.md)\
[前回の記事 05_move_raspi_using_kyori_sensor.md](./05_move_raspi_using_kyori_sensor.md)\

## 内容
1. [Part5_シミュレータ上のラズパイマウスを動かす方法](#Part5_シミュレータ上のラズパイマウスを動かす方法)

### 参考
* [Raspberry Pi Mouse Simulator's Tutorial](https://raspimouse-sim-tutorial.gitbook.io/project/)
* [how_to_install_simulator_xenial](https://github.com/rt-net/raspimouse_sim/wiki/how_to_install_simulator_xenial) 
* [作者ブログ](https://products.rt-net.jp/micromouse/archives/tag/ros)

### 次回やること
* シミュレーションのチュートリアルを読む
* ROSサービスの調査

### 所感　時間がかかったこと、難しかった、やりたいことなど


### 概要
[前回の記事](./05_move_raspi_using_kyori_sensor.md)の修正を行った。\
主にLefthandクラスを作成したことと、Nodehandleの変数を３つから１つにした

### 構成
-/src       -main.cpp       ::main部分、lefthand.run()を呼ぶだけ
            -lefthand.cpp   ::前回のソースコードをクラス化した
-/include   -lefthand.h     ::上のヘッダファイル
            -config.h       ::適当に作ったけど意味が無いです

### クラスにするときに注意するところ
* コールバックのクラスメソッドの使い方(subscriber)
    lefthand.cppの一部
    ```
    //光センサーの値をsubscribeする
    sub_lightsensor = nh.subscribe("/lightsensors", 10, &Lefthand::lightsensor_callback, this);
    ```
    [参考][ROS.org Using Class Methods as Callbacks](https://wiki.ros.org/roscpp_tutorials/Tutorials/UsingClassMethodsAsCallbacks)

* サービスを呼ぶ時も同じ
#### main.cpp
```
#include <ros/ros.h>
#include <ros/package.h> 
#include <std_msgs/String.h>
#include "../include/lefthand.h"

// TODO 重要：gazeboプロセスが落ちた時に死ぬ
// TODO 以前のプロセスが残ってたら殺す
// TODO エラーチェック

int main(int argc, char** argv){
    // 光センサーの値から壁を見つけて迷路の中を走るプログラム
    ros::init(argc, argv, "lefthand");
    Lefthand lefthand;
    lefthand.run();
    return 0;
}
```
#### lefthand.cpp
```
#include "ros/service_traits.h"
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/String.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/Empty.h>
#include "../include/lefthand.h"

void Lefthand::lightsensor_callback(const raspimouse_ros_2::LightSensorValues& msg){
    // 光センサの値を保存する
    lightsensor = msg;
}

bool Lefthand::motor_cont(const int left_hz, const int right_hz){
    // 車輪に指定された周波数を送って動かす
    if(ros::ok()){
        motor_freq.right_hz = right_hz;
        motor_freq.left_hz = left_hz;
        motor_freq_pub.publish(motor_freq);
        return 0;
    }else{ return 1; }
}

bool Lefthand::turn_move(std::string mode){
    // 左か右に旋回する
    // modeがLEFTの時は左旋回でRIGHTの時は右旋回
    if( mode == "LEFT"){ motor_cont(-200, 200);
    }else if( mode == "RIGHT" ){ motor_cont(200, -200);
    }else{
        ROS_ERROR("turn_move::Invalid Value :mode");
        return 1;
    }
    return 0;
}

bool Lefthand::moveFeedback(int offset, int speed, float k, std::string mode){
    // 左右の壁の距離から道の真ん中を走るようにする？？？？
    int diff = 0;
    // 左の壁が近い時、１度右旋回する
    if( lightsensor.left_side > 1500 ){
        turn_move("RIGHT");
        return 0;
    }
    // 右の壁が近い時、１度左旋回する
    if( lightsensor.right_side > 1500 ){
        turn_move("LEFT");
        return 0;
    }

    // 壁沿いを追従走行するための計算 (基準値 - 現在のleft_side) * ゲイン？？
    // 左モードの時
    if( mode == "LEFT" ){
        diff = (offset - lightsensor.left_side) * k;
        motor_cont(speed - diff, speed + diff);
    }else if( mode == "RIGHT"){ 
    // 右モードの時
        diff = (offset - lightsensor.right_side) * k;
        motor_cont(speed + diff, speed - diff);
    }else{
        ROS_ERROR("moveFeedback::Invalid Value :mode");
        return 1;
    }
    return 0;
}

bool Lefthand::stopMove(){
    // 車輪を止める
    motor_cont(0, 0);
    return 0;
}

bool Lefthand::checker(){
    // 壁なしチェックする？？
    // 左側面の光センサの値が１００より小さい、左の壁無いときrs_countを増やす
    if( lightsensor.left_side < 100){
        //ROS_INFO("---RS_COUNT:[%i]", lightsensor.left_side);
        rs_count += 1;
    }
    // 右側面の光センサの値が１００より小さい、右の壁無いときrs_countを増やす
    if( lightsensor.right_side < 150){
        //ROS_INFO("---LS_COUNT:[%i]", lightsensor.right_side);
        rs_count += 1;
    }
    return 0;
}

bool Lefthand::motion(){
    // ラズパイを操作する関数、実際にpublishするのはmotor_cont関数
    // 光センサの値で条件分けして左手法で動く
    // ROS_INFO("motion: ls [%i] lf [%i] rf [%i] rs [%i]", lightsensor.left_side, lightsensor.left_forward, lightsensor.right_forward, lightsensor.right_side );
    ros::Rate loop_rate(HZ);

    // 左側に壁がある確率が高くて目の前に壁がなさそうなとき
    if( lightsensor.left_forward < 300 || lightsensor.right_forward < 300 ){
        // 左右前の光センサの値が300より小さいとき、直進する
        ROS_INFO("Move: STRAIGHT");
        for( int i = 0; i < 12 ; i++ ){
            // 左右の壁なしチェックする
            checker();
            // 左の壁の方が右の壁より近い時
            if( lightsensor.left_side > lightsensor.right_side ){
                moveFeedback(500, 500, 0.2, "LEFT");
            }else{
                //右の壁のほうが近い時
                moveFeedback(500, 500, 0.2, "RIGHT");
            }
            loop_rate.sleep();
        } //for( time in range(12)){
        stopMove();

        // 直進したあと、前の壁がない？とき
        if( lightsensor.left_forward < 300 || lightsensor.right_forward < 300 ){
            // 左の壁がないとき左旋回する
            if( rs_count > 0 ){
                ROS_INFO("Move: MID LEFT TURN");
                for( int i = 0; i < 10 ; i++ ){
                    turn_move("LEFT");
                    loop_rate.sleep();
                }
                stopMove();
            }
        //直進したあと、前に壁があるとき
        }else if( lightsensor.left_forward > 300 && lightsensor.right_forward > 300 ){
            // 左右の壁がないとき
            if( ls_count > 0 && rs_count > 0 ){
                ROS_INFO("Move: LEFT TURN_2");
                for( int i = 0; i < 10 ; i++ ){
                    turn_move("LEFT");
                    loop_rate.sleep();
                } //for( time in range(10)){
                stopMove();
            // 右の壁がないとき
            }else if( ls_count > 0 ){
                ROS_INFO("Move: RIGHT TURN");
                for( int i = 0; i < 10 ; i++ ){
                    turn_move("RIGHT");
                    loop_rate.sleep();
                } //for( time in range(10)){
                stopMove();
            // 左の壁がないとき
            }else if( rs_count > 0 ){
                ROS_INFO("Move : LEFT TURN");
                for( int i = 0; i < 10 ; i++ ){
                    turn_move("LEFT");
                    loop_rate.sleep();
                } //for( time in range(10)){
                stopMove();
            }
        } // else if( lightsensor.left_forward > 300 && lightsensor.right_forward > 300 ){
        ls_count = 0;
        rs_count = 0;
        return 0;
    }

    // 目の前に壁があるとき左旋回する
    if( lightsensor.left_forward > 2000 && lightsensor.right_forward > 2000 ){
        ROS_INFO("Move: DEAD END");
        for( int i = 0; i < 20 ; i++ ){
            turn_move("LEFT");
            loop_rate.sleep();
        } 
        stopMove();
        ls_count = 0;
        rs_count = 0;
        return 0;
    }
    // 左の壁のほうが右の壁より近いとき
    if( lightsensor.left_side > lightsensor.right_side ){
        moveFeedback(500, 500, 0.2, "LEFT");
    // 右の壁のほうが左の壁より近い時
    }else{
        moveFeedback(500, 500, 0.2, "RIGHT");
    }

    return 0;
}

bool Lefthand::init(ros::NodeHandle& nn){
    // gazeboワールドの初期化とモータの通電を行う関数

    // ROSサービスを呼ぶためにNodehandleの変数を参照渡しで受け取る
    // gazebo/reset_worldとmotor_onのサービスを呼ぶ
    // http://pynote.hatenablog.com/entry/cpp_service
f
    if( mode_sim_reset ){
        ///gazebo/reset_worldが呼ばれるまで待つ
        ros::ServiceClient resetGazeboClient = nn.serviceClient<std_srvs::Empty>("gazebo/reset_world");
        std_srvs::Empty resetGazeboSrv;
        resetGazeboClient.waitForExistence();
        //motor_onサービスを呼ぶ
        if(resetGazeboClient.call(resetGazeboSrv)){
            ROS_INFO("Success to call service gazebo/reset_world");
        }else{
            ROS_ERROR("Failed to call service gazebo/reset_world");
            return 1;
        }
        mode_sim_reset = false; // サンプルプログラムにはなかったが追加した、わからん   
    }
    
    // モータの通電
    ros::ServiceClient motorClient = nn.serviceClient<std_srvs::Trigger>("motor_on");
    std_srvs::Trigger motorSrv;
    //motor_onのサービスが存在するまで待つ
    motorClient.waitForExistence();
    //motor_onサービスを呼ぶ
    if(motorClient.call(motorSrv)){
        ROS_INFO("Success to call service motor_on");
    }else{
        ROS_ERROR("Failed to call service motor_on");
        ROS_ERROR("message:[%s]", motorSrv.response.message);
        return 1;
    }
    return 0;
}

bool Lefthand::run(){
    // メイン、ループで動く部分
    ros::Rate loop_rate(HZ);

    // 光センサの値をサブスクライブするまでsleepで待つ
    while(lightsensor.left_side == 0 && lightsensor.right_side == 0){        
        ros::spinOnce();
        loop_rate.sleep();
    }
    // ラズパイを動かす
    while (ros::ok()) {
        motion();
        loop_rate.sleep();    
        ros::spinOnce();
    }
}

Lefthand::Lefthand(){
    // コンストラクタ、初期化をする
    ROS_INFO("Launch lefthand.cpp");

    // 光センサとモータの値を初期化する
    mode_sim_reset = true;
    lightsensor.left_forward = 0;
    lightsensor.left_side = 0;
    lightsensor.right_forward = 0;
    lightsensor.right_side = 0;
    motor_freq.left_hz = 0;
    motor_freq.right_hz = 0;

    // gazeboワールドの初期化とモータの通電用関数
    init(nh);

    //モータの周波数をパブリッシュする
    motor_freq_pub = nh.advertise<raspimouse_ros_2::MotorFreqs>("/motor_raw", 10);  // 10 is magic num

    //光センサーの値をsubscribeする
    sub_lightsensor = nh.subscribe("/lightsensors", 10, &Lefthand::lightsensor_callback, this);
}

Lefthand::~Lefthand(){
    // デスコントラクタ、終了処理をする
    // モータを止める
    stopMove();
}
```
#### lefthand.h
```
#ifndef _INC_LEFTHAND
#define _INC_LEFTHAND

#include <string>
#include <iostream>
#include <ros/ros.h>
#include <ros/package.h>
 
#include "config.h"
// 光センサの値を受け取るためのカスタムメッセージ
#include "raspimouse_ros_2/LightSensorValues.h"
//　モータの値を送信するためのカスタムメッセージ
#include "raspimouse_ros_2/MotorFreqs.h"

class Lefthand
{
private:
    // 車輪に周波数をPublishする
    ros::Publisher motor_freq_pub;
    // 光センサの値をSubscribeする
    ros::Subscriber sub_lightsensor;

    // Publish周期は10Hz
    const float HZ = 10;

    // gazenoワールドのリセット用　needs_sim_reset的な意味、リセットしたらfalse
    bool mode_sim_reset = true;

    // 光センサの値を保存する
    raspimouse_ros_2::LightSensorValues lightsensor;
    // motor_rawトピックに送信する車輪の周波数を保存する
    raspimouse_ros_2::MotorFreqs motor_freq;

    // 壁なし判定で何かに使ってインクリメントする変数
    int rs_count = 0;
    int ls_count = 0;
    
    bool motor_cont(const int left_hz, const int right_hz);
    bool turn_move(std::string mode);
    bool moveFeedback(int offset, int speed, float k, std::string mode);
    bool stopMove();
    bool checker();
    bool motion();
    bool init(ros::NodeHandle& nn);

public:
    Lefthand();
    ~Lefthand();
    bool run();
    void lightsensor_callback(const raspimouse_ros_2::LightSensorValues& msg);
    ros::NodeHandle nh;
};

#endif	//INC_LEFTHAND
```
#### config.h
なんの意味もないです
```
#ifndef LEFTHAND_CONFIG_H_
#define LEFTHAND_H_

#include <string>
#include <iostream>

const std::string HOME_PATH = "/home/XXX/";
const std::string VERSION   = "0.88";
const bool isDebug  = false;

#endif  // SOME_CONFIG_H_
```
#### CMakeLists.txt
変更した部分のみ記載
1. include_deirectoriesでヘッダファイルを読む
2. add_executableにmain.cppとlefthand.cppを追加
```
###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

set(SOURCE_FILES
  src/main.cpp
  src/lefthand.cpp
)

add_executable(lefthand ${SOURCE_FILES})
```
[トップへ](#内容)

<!--
```
プログラムを書く
```
-->

