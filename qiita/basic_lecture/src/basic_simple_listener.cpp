#include "ros/ros.h"
#include "std_msgs/String.h"

void chatterCallback(const std_msgs::String& msg){ //対象トピックが更新されたら呼び出されるコールバック関数
    ROS_INFO("I Heard [%s]", msg.data.c_str());
}

int main(int argc, char **argv){
    ros::init(argc, argv, "listener");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("chatter", 10, chatterCallback);
    ros::Rate loop_rate(10);

    ros::spin();
    return 0;
}