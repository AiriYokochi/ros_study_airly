//Broadcaster用のクラス
#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>   //tf::TransformBroadcaste用
#include <string>
#include <math.h>

int main(int argc, char **argv){
    
    ros::init(argc, argv, "vis_tf_publisher");
    ros::NodeHandle nh;

    ros::Rate loop_rate(10);
    int count = 0;
    while (ros::ok())
    {
        float xyz[3]={0,0,0};           //座標を格納する
        xyz[0]=cos((float)count/5.0);   //0.2ずつcosの中身を増やす、約30フレームで1回転
        xyz[1]=sin((float)count/5.0);

        std::string source_frame="base_link";
        std::string target_frame="body_link";
        geometry_msgs::Pose t_pose;
        t_pose.position.x=xyz[0];
        t_pose.position.y=xyz[1];
        t_pose.orientation.w=1.0;               //0度

        static tf::TransformBroadcaster br;
        tf::Transform transform;
        poseMsgToTF(t_pose, transform);         //t_poseをTFに変換
        br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), source_frame, target_frame));
        //引数1  transform自体を渡す
        //引数2  現在の時間をtimestampとして渡す
        //引数3  今作っているリンクの親フレームの名前を渡す
        //引数4  今作っているリンクの小フレームの名前を渡す、自身のリンク　base_jointの挙動？

        count++;
        ros::spinOnce();
        loop_rate.sleep();
    }
    
    return 0;
}