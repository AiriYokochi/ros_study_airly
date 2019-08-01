#include "ros/ros.h"
#include "basic_lecture/cmsg.h"

int main(int argc, char **argv){
    /* 初期化 */
    //以下2行はお約束みたい
    ros::init(argc, argv, "simple_talker"); //自分のノード名をつける
    ros::NodeHandle n;                      //ノードハンドラ

    ros::Publisher pub = n.advertise<basic_lecture::cmsg>("chatter", 100);
    //String型のメッセージをchatterというトピックへ配信するキューは10
    
    /* Hello Worldを送信する */
    ros::Rate loop_rate(1);
    //10秒ごとに1メッセージを配信＿
    while (ros::ok()){                  //ノード実行中は基本的にros::ok()=1
        basic_lecture::cmsg data;
        data.word = "Hello World";//わからん
        data.number=10;
        pub.publish(data);//publiserのAPI
        ros::spinOnce();//コールバックが呼ばれる　spinOneの周期=この行の実行周期
        loop_rate.sleep();//spinOnceが呼ばれてからコールバックが呼ばれるまで遅延があるため

    }
    return 0;
}

