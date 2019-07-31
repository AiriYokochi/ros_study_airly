# Raspberry Pi Mouse Simulator's Tutorial for airy

[前回の記事 01_setup.md)](./01_setup.md)\
[前回の記事 00_Inventor.md](./00_Inventor.md)\
[前回の記事 02_kyori_sensor_test.md](./02_kyori_sensor_test.md)
[前回の記事 05_kyori_sensor_test.md](./05_kyori_sensor_test.md)
[前回の記事 05_kyori_sensor_test_Part2.md](./05_kyori_sensor_test_Part2.md)
[前回の記事 06_slam_using_urg.md](./06_slam_using_urg.md)
## 内容
1. [Part6_シミュレータ上のラズパイマウスを動かす方法](#Part6_シミュレータ上のラズパイマウスを動かす方法)

### 参考
* [Raspberry Pi Mouse Simulator's Tutorial](https://raspimouse-sim-tutorial.gitbook.io/project/)
* [how_to_install_simulator_xenial](https://github.com/rt-net/raspimouse_sim/wiki/how_to_install_simulator_xenial) 
* [作者ブログ](https://products.rt-net.jp/micromouse/archives/tag/ros)
* [Github arpg/Gazebo/world/willowgarage.world](https://github.com/arpg/Gazebo/blob/master/worlds/willowgarage.world)
* [Running turtlebot simulator in willow garage world](https://answers.ros.org/question/12956/running-turtlebot-simulator-in-willow-garage-world/)
* [Slideshare ROSを用いた自律移動ロボットのシステム構築](https://www.slideshare.net/hara-y/ros-nav-rsj-seminar)

### 所感　時間がかかったこと、難しかった、やりたいことなど
[参考][HOKUYO トップページ->製品の原理 用語解説->測域センサ->測域センサ](https://www.hokuyo-aut.co.jp/products/data.php?id=41)
[所感]測域センサ使ってみたい！！！！

## Part6_シミュレータ上のラズパイマウスを動かす方法
### 概要
[前回の記事](./06_slam_using_urg.md)で動作させたサンプルプログラムの理解を行う\

### raspimouse_with_willowgarage.launch
ラズパイマウスとwillowgarageをgazebo上に表示させる
```
<?xml version="1.0"?>
<launch>
  <rosparam file="$(find raspimouse_control)/config/controller.yaml" command="load" />
  <arg name="model" default="$(find raspimouse_description)/robots/raspimouse_urg.urdf.xacro" />
  <arg name="paused" default="false" />
  <arg name="use_sim_time" default="true" />
  <arg name="gui" default="true" />
  <arg name="headless" default="false" />
  <arg name="debug" default="false" />

  <include file="$(find gazebo_ros)/launch/willowgarage_world.launch"/>

  <!-- Load the URDF into the ROS Parameter Server -->
  <param name="raspimouse_on_gazebo/robot_description" command="$(find xacro)/xacro --inorder '$(arg model)'" />
  
  <!-- Run a python script to the send a service call to gazebo_ros to spawn a URDF robot -->
  <node name="urdf_spawner" pkg="gazebo_ros" type="spawn_model" respawn="false" output="screen" args="-urdf -model RasPiMouseV2 -param raspimouse_on_gazebo/robot_description" />

  <!-- ros_control motoman launch file -->
  <include file="$(find raspimouse_control)/launch/raspimouse_control.launch" />
</launch>
```
### raspimouse_sim_gmapping.launch
1. run_with_base_nodes.launchを起動してjoyスティックでラズパイマウスを動かせるようにする
2. rvizの起動
3. slam_gmappingノードの実行
  パラメーター各種
  * base_frame:ロボットのメインフレーム
  * odom_frame:おどめ取りのフレーム
  * maxUrange:URGの最大測定範囲
  * xmin,ymin,xmax,ymax:地図の大きさ
  * srr:オドメトリの誤差
  * srt:
  * srt:
  * stt:
  * particles:パーティクルの数
```
<launch>
 <include file="$(find raspimouse_game_controller)/launch/run_with_base_nodes.launch" />

 <node pkg="rviz" type="rviz" name="rviz" args="-d $(find raspimouse_sim_tutorial_program)/config/gmapping.rviz" /> 

 <node pkg="gmapping" type="slam_gmapping" name="slam_gmapping" output="screen">
  <param name="base_frame" value="base_link" />
  <param name="odom_frame" value="odom" />

  <param name="maxUrange" value="4.0" />
  <param name="maxRange" value="4.0" />

  <param name="xmin" value="-15" />
  <param name="ymin" value="-15" />
  <param name="xmax" value="15" />
  <param name="ymax" value="15" />

  <param name="srr" value="0.1" />
  <param name="srt" value="0.1"/>
  <param name="str" value="0.1"/>
  <param name="stt" value="0.1"/>

  <param name="particles" value="30" />
 </node>
</launch>
```

[トップへ](#内容)

<!--
```
プログラムを書く
```
-->

