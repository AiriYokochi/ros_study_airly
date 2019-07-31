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