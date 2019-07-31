#include "ros/ros.h"
#include "std_msgs/String.h"

int main(int argc, char **argv){
    /* 初期化 */
    //以下2行はお約束みたい
    ros::init(argc, argv, "simple_talker"); //自分のノード名をつける
    ros::NodeHandle n;                      //ノードハンドラ
    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 10);
    //String型のメッセージをchatterというトピックへ配信するキューは10
    
    /* Hello Worldを送信する */
    ros::Rate loop_rate(10);
    //10秒ごとに1メッセージを配信＿
    while (ros::ok()){                  //ノード実行中は基本的にros::ok()=1
        std_msgs::String msg;
        msg.data = "Hello World";//わからん
        ROS_INFO("%s", msg.data.c_str());
        chatter_pub.publish(msg);//publiserのAPI
        ros::spinOnce();//コールバックが呼ばれる　spinOneの周期=この行の実行周期
        loop_rate.sleep();//spinOnceが呼ばれてからコールバックが呼ばれるまで遅延があるため

    }
    return 0;
}

