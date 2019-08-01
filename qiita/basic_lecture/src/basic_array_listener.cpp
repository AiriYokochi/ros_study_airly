#include "ros/ros.h"
#include "std_msgs/Float32MultiArray.h"

void chatterCallback(const std_msgs::Float32MultiArray& msg){ //対象トピックが更新されたら呼び出されるコールバック関数
    int num=msg.data.size();
    ROS_INFO("I subsclibed [%i]", num);
    for(int i=0; i<num; i++){
        ROS_INFO("[%i]:%f", i, msg.data[i]);
    }
}

int main(int argc, char **argv){
    ros::init(argc, argv, "basic_array_listener");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("array", 1000, chatterCallback);
    ros::Rate loop_rate(10);

    ros::spin();
    return 0;
}