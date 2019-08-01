#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_listener.h>

#include <string>
#include <math.h>

int main(int argc, char **argv){
    ros::init(argc, argv, "vis_tf_listen");
    ros::NodeHandle nh;

    tf::TransformListener ln;   //実体化のタイミングはnodehandleの宣言後、グローバル空間はNG

    ros::Rate loop_rate(10);

    while(ros::ok()){
        geometry_msgs::PoseStamped source_pose; //たどる元リンクの位置？？？
        source_pose.header.frame_id="body_link";
        source_pose.header.stamp=ros::Time(0);  //時刻を保存して指定した時間以降のtfが他でpublishするまで待つ　0は最新
        source_pose.pose.orientation.w=1.0;

        geometry_msgs::PoseStamped target_pose; //たどる先リンクの位置？？？
        std::string target_frame="base_link";

        //起動しての場合などtfが存在しない時に一定時間待ったあとにタイムアウトして例外を出すので必ずtry-catch
        try{
            ln.waitForTransform(source_pose.header.frame_id, target_frame, ros::Time(0), ros::Duration(1.0));
            //時間を指定してtfが出てくるまで待つ

            ln.transformPose(target_frame, source_pose, target_pose);
            //座標変換を行う
            ROS_INFO("x:%+5.2f, y:%+5.2f, z:%+5.2f", target_pose.pose.position.x, target_pose.pose.position.y,target_pose.pose.position.z);
        }catch(...){
            ROS_INFO("tf error");
        }
        ros::spinOnce();
        loop_rate.sleep();

    }
    return 0;
}