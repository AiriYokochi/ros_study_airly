//#include "/opt/ros/kinetic/include/省略されてるよ
#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"

int main(int argc, char **argv){
    /* 初期化 */
    //以下2行はお約束みたい
    ros::init(argc, argv, "basic_array_talker"); //自分のノード名をつける
    ros::NodeHandle n;                      //ノードハンドラ
    ros::Publisher pub = n.advertise<std_msgs::Float32MultiArray>("array", 10);
    //String型のメッセージをchatterというトピックへ配信するキューは10
    
    ros::Rate loop_rate(1);
    //1秒ごとに1メッセージを配信＿
    while (ros::ok()){                  //ノード実行中は基本的にros::ok()=1
        std_msgs::Float32MultiArray array;
        array.data.resize(4);
        array.data[0]=0.0;
        array.data[1]=1.0;
        array.data[2]=2.0;
        array.data[3]=3.0;
        pub.publish(array);
        ROS_INFO("publish array done");
        ros::spinOnce();//コールバックが呼ばれる　spinOneの周期=この行の実行周期
        loop_rate.sleep();//spinOnceが呼ばれてからコールバックが呼ばれるまで遅延があるため

    }
    return 0;
}

