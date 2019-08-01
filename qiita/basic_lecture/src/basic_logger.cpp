#include "ros/ros.h"
#include <ros/console.h>
#include <string.h>

int main(int argc, char **argv){
    ros::init(argc, argv, "logger");
    ros::NodeHandle n;
    ros::Rate loop_rate(1);
    if( ros::console::set_logger_level(ROSCONSOLE_DEFAULT_NAME, ros::console::levels::Info) ) {
        ros::console::notifyLoggerLevelsChanged();
    }


    int count = 1;
    while(ros::ok() && count <= 10){
        ROS_DEBUG("--log:%i", count);
        ROS_INFO("---log:%i", count);
        ROS_WARN("----log:%i", count);
        ROS_ERROR("log:%i", count);
        ROS_FATAL("log:%i", count);
        ROS_INFO("AIREEE");
        count++;
        ros::spinOnce();
        loop_rate.sleep();

    }
//    count++;
    return 0;

}