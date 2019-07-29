# Raspberry Pi Mouse Simulator's Tutorial for airy

[前回の記事 01_setup.md)](./01_setup.md)\
[前回の記事 00_Inventor.md](./00_Inventor.md)\
[前回の記事 02_kyori_sensor_test.md](./02_kyori_sensor_test.md)

## 内容
1. [ROSサービスとは](#ROSサービスとは)
2. [Part5_シミュレータ上のラズパイマウスを動かす方法](#Part5_シミュレータ上のラズパイマウスを動かす方法)

### 参考
* [Raspberry Pi Mouse Simulator's Tutorial](https://raspimouse-sim-tutorial.gitbook.io/project/)
* [how_to_install_simulator_xenial](https://github.com/rt-net/raspimouse_sim/wiki/how_to_install_simulator_xenial) 
* [作者ブログ](https://products.rt-net.jp/micromouse/archives/tag/ros)

### 次回やること
* シミュレーションのチュートリアルを読む
* ROSサービスの調査

### 所感　時間がかかったこと、難しかった、やりたいことなど
[調べる] xacroからworldに変換する方法について、xacroのオプションについて\
[調べる] Warning [parser.cc:759] XML Attribute[xmlns:xacro] in element[sdf] not defined in SDF, ignoring. \
[解決]　ERROR: Service [/motor_on] is not available. -> raspi_ros_2の起動をする\
[参考][ROSのサービスとパラメータ](http://hara.jpn.com/_default/ja/Topics/ROS%E3%81%AE%E3%82%B5%E3%83%BC%E3%83%93%E3%82%B9%E3%81%A8%E3%83%91%E3%83%A9%E3%83%A1%E3%83%BC%E3%82%BF.html)\
[参考][C++でシンプルなサービスとクライアントを書く](http://wiki.ros.org/ja/ROS/Tutorials/WritingServiceClient%28c%2B%2B%29)
[参考][ros_about_spin](https://gist.github.com/KobayashiRui/1dd3f69b48d51ced9ff8628a2fa1bbb6)
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
/motor_onのサービスが無いことがわかる\
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

レスポンスの詳細を見るにはtypeコマンドの結果をrossrv showにパイプする
```
$ rosservice type /motor_on | rossrv show
---
bool success
string message
```
レスポンスは２つ
1. bool型success::raspimouse_with_samplemaze.launch
2. string型のmessage::メッセージ(これは何)

[トップへ](#内容)

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

### サンプルPythonの理解
#### rosnode info
```
rosnode info /LeftHand
--------------------------------------------------------------------------------
Node [/LeftHand]
Publications: 
 * /motor_raw [raspimouse_ros_2/MotorFreqs]
 * /rosout [rosgraph_msgs/Log]

Subscriptions: 
 * /clock [rosgraph_msgs/Clock]
 * /lightsensors [raspimouse_ros_2/LightSensorValues]

Services: 
 * /LeftHand/get_loggers
 * /LeftHand/set_logger_level


contacting node http://192.168.12.114:34011/ ...
Pid: 2735
Connections:
 * topic: /rosout
    * to: /rosout
    * direction: outbound
    * transport: TCPROS
 * topic: /motor_raw
    * to: /motors
    * direction: outbound
    * transport: TCPROS
 * topic: /clock
    * to: /gazebo (http://192.168.12.114:41877/)
    * direction: inbound
    * transport: TCPROS
 * topic: /lightsensors
    * to: /lightsensors (http://192.168.12.114:38219/)
    * direction: inbound
    * transport: TCPROS
```
[rqt_graphの結果](/images/lefthand.py)\
raspimouse_ros_2/MotorFreqs型のモーター周波数をパブリッシュすることと、\
rosgraph_msgs/Clock型のgazebo上の時間とraspimouse_ros_2/LightSensorValues型の光センサーの値をサブスクライブすることがわかる


#### rostopic echo
* /lightsensors 
```
right_forward: 172
right_side: 72
left_side: 2843
left_forward: 4000
sum_all: 7087
sum_forward: 4172
```
* /motor_raw
```
left_hz: 200
right_hz: -200
```

#### 流れ
##### mainクラス
ros::initをする
LeftHandクラスのrunメソッドを実行
##### LeftHandクラス
左手法で迷路を進むプログラム
###### クラス変数
* data::MotorFreqs型
    * left_hz::int型 左輪の周波数
    * right_hz::int型　右輪の周波数
    * left_side::int型　光センサーの値
    * right_side::int型　光センサーの値
    * left_forward::int型　光センサーの値
    * right_forward::int型　光センサーの値
* modeSimReset::bool型    シミュレータを初期状態にする？
* ls_count::int型
* rs_count::int型

###### メソッド
1. コンストラクタ::
    * LeftHandクラスがインスタンス時に呼ばれる
        * 光センサの値のサブスクライブをセット
        * モータに周波数を入力するパブリッシャーセット
        * シミュレータを初期状態にする
        * ls_count,rs_countを0に初期化する

2. sensor_callback::
    * 引数は1, data:LightSensorValues型のデータ
    * 光センサーの値をサブスクライブした時に呼ばれる
        * dataにLightSensorValues型のメッセージを保存する

3. motor_cont::
    * 引数は2, left_hz,right_hz:int型の左右輪の周波数
    * どんなときにどこから呼ばれる
        * rospyがシャットダウンしていない時に両輪の周波数を設定する
        * 設定した周波数をパブリッシュする

4. turn_move::
    * 引数は1, m:String型のデータで"LEFT", "RIGHT"が入る
        * LEFTの場合は左回りになるように両輪の周波数を設定しmotor_contを呼ぶ
        * RIGHTの場合は右回りになるように両輪の周波数を設定しmotor_contを呼ぶ

5. move_Feedback::
    * 引数は4
        * offset:基準値, speed k：ゲイン, mode:左旋回中か右旋回中か 
        * 左側面の光センサの値が大きい時、左に壁があるとき右旋回する
            * data.left_sideが1500より大きい時はturn_moveを呼ぶ
        * 右側面の光センサの値が大きい時、右に壁があるとき左旋回する
            * data.right_sideが1500より大きい時はturn_moveを呼ぶ
        * 壁沿いを追従走行する
            * "LEFT"modeのとき、差分を計算してモータに出力？
                * 差分は(基準値-左側面の光センサの値)* ゲイン
                * モータに出力する値は右輪に(現在の速度-差分)　左輪に(現在の速度+差分)
            * "RIGHT"modeのとき、差分を計算してモータに出力？
                * 差分は(基準値-右側面の光センサの値)* ゲイン
                * モータに出力する値は右輪に(現在の速度+差分)　左輪に(現在の速度-差分)

6. stopMove::
    * 終了時にモータを止める

7. checker::
    * 壁なし判定をする
        * left_sideが100より小さい
            * RS_COUNTでleft_sideを標準出力する？
            * rs_countをインクリメントする
        * right_sideが150より小さい
            * LS_COUNTでright_sideを標準出力する
            * ls_countをインクリメントする

8. motion::
    * 動作部分
        * 左側に壁がある確率が高くて目の前に壁がなさそうなとき
            * "Move:STRAIGHT"を標準出力
            * for time in range(12) 12単位分の時間だけ
                * 壁なし判定をする
                * 左の壁が右の壁より近いとき、move_Feedbackを呼ぶ
                    * 基準値500 スピード500 ゲイン0.2 モードLEFT
                * 右の壁が左の壁より近いとき、move_Feedbackを呼ぶ
                    * 基準値500 スピード500 ゲイン0.2 モードRIGHT
                * 待つ
            * モータを止める

            * 目の前に壁がなくて右に壁がない場合
                * rs_countが0より大きければ"Move :MID LEDT TURN"標準出力
                * 10単位分の時間だけ
                    * 左旋回して待つ
                * モータを止める
            * 目の前に壁があった時
                * 左右に壁がない場合は
                    * rs_count ls_countが0より大きければ"Move: LEFT TURN_2"を標準出力
                    * 10単位分の時間だけ左旋回して待つ
                    * モータを止める
                * ls_countが0より大きい場合、右の壁がない場合は"Move: RIGHT TURN"と出力して
                    * 10単位分の時間だけ右旋回して待つ
                    * モータを止める
                * rs_countが０より大きい、左の壁がない場合は"Move: LEFT TURN"と出力して
                    * １０単位分左旋回して待つ
                    * モータを止める
            * ls_count, rs_countを０で初期化する
            * return 
        * 左右関係なく目の前に壁があるとき
            * "Move:DEAD END"と出力し２０単位分の時間だけ左旋回して待つ
            * モータを止める
            * ls_count, rs_countを０で初期化する
            * return
        * 左の壁が右の壁より近いとき、move_Feedbackを呼ぶ
            * 基準値500 スピード500 ゲイン0.2 モードLEFT
        * 右の壁が左の壁より近いとき、move_Feedbackを呼ぶ
            * 基準値500 スピード500 ゲイン0.2 モードRIGHT

9. init::
    * gazeboワールドのリセットとモータの通電を行う
        * modeSimResetができている時
            * /gazebo/reset_worldサービスが使えるまで待つ
            * /gazebo/reset_worldのサービスをEmptyで呼ぶ
                * エラーキャッチしたら内容を表示
            * /motor_onのサービスが使えるまで待つ
            * /motor_onのサービスをTriggerで呼ぶ
                * エラーキャッチしたら内容を表示
10. run::
    * 10Hzで設定
    * initを呼んでgazeboワールドのリセットとモータの通電を行う
    * on_shutdown？でstopMoveを呼んでモータを止める
    * 左右側面の壁の光センサの値が０の時、待ち続ける
    * シャットダウンしていない時、motionを呼ぶ、10Hzでまつ


#### lefthand.cpp
今回作成したCPPファイル
```
#include <ros/ros.h>
#include <ros/package.h> 
#include "ros/service_traits.h"
#include <geometry_msgs/Twist.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/String.h>
#include <std_srvs/Trigger.h>
#include <std_srvs/Empty.h>

// 光センサの値を受け取るためのカスタムメッセージ
#include "raspimouse_ros_2/LightSensorValues.h"
//　モータの値を送信するためのカスタムメッセージ
#include "raspimouse_ros_2/MotorFreqs.h"

//カスタムメッセージをインクルードする ROS43
// ROS講座 43 https://qiita.com/srs/items/525e24634c638635e034

ros::Publisher motor_freq_pub;

// pub周期は10Hz
const float HZ = 10;

// シミュレータのリセット用？なんの時に何？
bool mode_sim_reset;

// 光センサの値を保存する
raspimouse_ros_2::LightSensorValues lightsensor;
// motor_rawトピックに送信する車輪の周波数を保存する
raspimouse_ros_2::MotorFreqs motor_freq;

int rs_count = 0;
int ls_count = 0;

void lightsensor_callback(const raspimouse_ros_2::LightSensorValues& msg){
    // 光センサの値を保存する
    lightsensor = msg;
}

int motor_cont(const int left_hz, const int right_hz){
    // 車輪に指定された周波数を送って動かす
    if(ros::ok()){
        motor_freq.right_hz = right_hz;
        motor_freq.left_hz = left_hz;
        motor_freq_pub.publish(motor_freq);
        return 0;
    }else{ return 1; }
}

int turn_move(std::string mode){
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

int moveFeedback(int offset, int speed, float k, std::string mode){
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

int stopMove(){
    // 車輪を止める
    motor_cont(0, 0);
    return 0;
}

int checker(){
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

int motion(){
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
                ROS_INFO("Move: LEFT TURN");
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

int init(){
    // gazeboワールドの初期化とモータの通電用関数
    // gazebo/reset_worldとmotor_onのサービスを呼ぶ
    // http://pynote.hatenablog.com/entry/cpp_service

    // [fix] nn作らずにやる方法
    ros::NodeHandle nn;
    
    // gazeboワールドのリセット
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

int main(int argc, char** argv){
    // サンプルプログラムのmainとrunを一緒にしてます
    // 光センサーの値から壁を見つけて迷路の中を走るプログラム

    // ROSの初期化
    ros::init(argc, argv, "lefthand");
    ros::NodeHandle n;
    ros::NodeHandle nh;
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
    init();

    //モータの周波数をパブリッシュする
    motor_freq_pub = n.advertise<raspimouse_ros_2::MotorFreqs>("/motor_raw", 1);

    //光センサーの値をsubscribeする
    ros::Subscriber sub_lightsensor = nh.subscribe("/lightsensors", 1, lightsensor_callback);

    ros::Rate loop_rate(HZ);

    // 光センサの値をサブスクライブするまでsleepで待つ
    while(lightsensor.left_side == 0 && lightsensor.right_side == 0){        
        ros::spinOnce();
        loop_rate.sleep();
    }
    // ラズパイを動かす
    while (ros::ok()) {
        motion();
        ros::spinOnce();
        loop_rate.sleep();    
    }
    // ROS終了処理
    stopMove();

    return 0;
}
```
#### launchファイル
* raspimouse_with_samplemaze.launchに以下追加
    * raspimouse_ros_2)/launch/raspimouse.launch
    * lefthand.cpp
```
<?xml version="1.0"?>
<launch>
  <!-- these are the arguments you can pass this launch file, for example paused:=true -->
  <rosparam file="$(find raspimouse_control)/config/controller.yaml" command="load" />
  
  <arg name="model" default="$(find raspimouse_description)/robots/raspimouse.urdf.xacro" />
  <arg name="paused" default="false" />
  <arg name="use_sim_time" default="true" />
  <arg name="gui" default="true" />
  <arg name="headless" default="false" />
  <arg name="debug" default="false" />

  <!-- We resume the logic in empty_world.launch, changing only the name of the world to be launched -->
  <include file="$(find gazebo_ros)/launch/empty_world.launch">
    <arg name="world_name" value="$(find raspimouse_gazebo)/worlds/sample_maze.world" />
    <arg name="debug" value="$(arg debug)" />
    <arg name="gui" value="$(arg gui)" />
    <arg name="paused" value="$(arg paused)" />
    <arg name="use_sim_time" value="$(arg use_sim_time)" />
    <arg name="headless" value="$(arg headless)" />
  </include>

  <!-- Load the URDF into the ROS Parameter Server -->
  <param name="raspimouse_on_gazebo/robot_description" command="$(find xacro)/xacro --inorder '$(arg model)'" />
  
  <!-- Run a python script to the send a service call to gazebo_ros to spawn a URDF robot -->
  <node name="urdf_spawner" pkg="gazebo_ros" type="spawn_model" respawn="false" output="screen" args="-urdf -model RasPiMouseV2 -param raspimouse_on_gazebo/robot_description" />

  <!-- ros_control motoman launch file -->
  <include file="$(find raspimouse_control)/launch/raspimouse_control.launch" />
  <include file="$(find raspimouse_ros_2)/launch/raspimouse.launch" />

  <node name="lefthand" pkg="lefthand" type="lefthand" output="screen"/>
</launch>

```
#### CMakeLists.txt
* パッケージにraspimouse_ros2の追加
```
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  raspimouse_ros_2
)
```
* 作成したCPPファイルの追加
```
add_executable(lefthand src/lefthand.cpp)
target_link_libraries(lefthand ${catkin_LIBRARIES})
```

#### メモ
##### Pythonとの違い
* データ型の定義
    * Python
        ```
        self.data = LightSensorValues()
        ```
    * C++
        ```
        raspimouse_ros_2::LightSensorValues lightsensor;
        ```
* ros::init
    * Python
        ```
        rospy.init_node('LeftHand')
        ```
    * C++
        ```
        ros::init(argc, argv, "lefthand");
        ```
* NodeHandle, SpinOnceの有無
    rospyにはNodeHandle,SpinOnceがない
* ros::ok
    * Python
        ```
        rospy.on_shutdown(self.stopMove)
        while not rospy.is_shutdown():
            self.motion()
            self.rate.sleep()
        ```
    * C++
        ```
        while (ros::ok()) {
            motion();
            ros::spinOnce();
            loop_rate.sleep();    
        }
        ```
##### ROSサービスの使い方
* include
    ```
    #include <std_srvs/Trigger.h>
    ```
* 実際のコード一部
ros::ServiceClientを使う
```
    ros::NodeHandle nn;
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
```
##### Pub&Subを両方やるとき
多分タイミングが合わないと値が入らない？後で調査
```
    //モータの周波数をパブリッシュする
    motor_freq_pub = n.advertise<raspimouse_ros_2::MotorFreqs>("/motor_raw", 1);
    //光センサーの値をsubscribeする
    ros::Subscriber sub_lightsensor = nh.subscribe("/lightsensors", 1, lightsensor_callback);
```
[トップへ](#内容)

<!--
```
プログラムを書く
```
-->

