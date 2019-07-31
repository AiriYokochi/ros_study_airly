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