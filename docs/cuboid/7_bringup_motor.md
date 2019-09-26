# 7_bringup_motor.md

[前記事　6_bringup_camera.md](6_bringup_camera.md)←　→[次記事 8_hoge.md](7_bringup_motor.md)

cuboidでmotorを起動する際の挙動などのメモ

## 参考リンク

* [ROS講座84 Nodeletを使う](https://qiita.com/srs/items/2718feee04643b2c8afa)
* [ROS講座16 joint_stateをpublishする](https://qiita.com/srs/items/3eb58fd32eee4d84a530)

## 疑問

* cube_controll/src/base_driver_node.cppは何？

## 開発環境

* Ubuntu 16.04LTS
* Linux kernel 4.15.0-58-generic

## 概要

ロボットの車輪が動くまで

* ControllerManager:controller_spawnerで起動する
    * diff_drive_controller
        * /cmd_velから/odomへ
        * 台車の目標速度(/cmd_vel)を入力
        * 台車の測定速度を出力
    * joint_state_controller
        * 関節角度を入力
        * joint_statesをpublish

* HardwareInterface:urdfで記述
    * VelocityJointInterface
    * 2車輪分

## bringupのlaunchファイル

実機起動する際はcuboid.launchを実行する。
モータ周りに関係するところのみ抽出する。

* robot_descriptionパラメータの設定
* joint_state_publisher
    * 関節角情報(/jointstate)をpublishする
* robot_state_publisher
    * /joint_stateトピックを受け取りrobot_descriptionパラメータを元に姿勢(/tf, /tf_static)をpublishする
* diff_drive_controller
    * cube_control/launch/base_control_cuboid.launchを参照
    * 他のlaunchファイルを読み込む際は```<include file="file_name"/>```
    * パラメータは2つ
        * left_wheel_port:::左車輪のポート名。/dev/tty0みたいな
        * right_wheel_port::右車輪のポート名
* velocity_smoother

## diff_drive_controllerのlaunchファイル

base_control_cuboid.launchの中身について

* 車輪パラメータの設定
    * left_wheel_port:::左車輪のポート名。/dev/tty0みたいな
    * right_wheel_port::右車輪のポート名
* rosparamでconfig/base_control_cuboid.yamlを読み込む
    * 書き方は```<rosparam file="file_name.yaml" command="load">```
* vesc_driver
    * vesc_driver/launch/vesc_driver_nodelet.launchを起動する
        * ns(namespace)はbase/wheel_leftと/wheel_right2つそれぞれで起動
            * パラメータ2つ
                * node_name::ROSノードの名前(vesc_wheel_r(l))
                * port::車輪のポート名
* controller_manager
    * joint_state_controllerとdiff_drive_controllerを起動する
    * joint_state_controllerはgazebo上のjointの角度を/joint_statesにpublishする
        * robot_state_publisherがtfに自動変換
    * 書き方 ```<node name="base_controller_spawner" pkg="controller_manager" type="spawner" respawn="false" output="screen" ns="base" args="diff_drive_controller joint_state_controller"/>```
* base_driver
    * nsはbase
    * cube_contrtolのbase_driverノードを立ち上げる

## base_driver_node

左右の車輪のタイミングを統一しながらread,update,write(何もしない),sleepを繰り返す

## base_control.cuboid.yaml

diff_drive_controller.yamlとjoint_state_controller.yamlの設定を両方書いている\
diff_drive_controller.yaml部分
```
base:
    diff_drive_controller:
        type: diff_drive_controller/DiffDriveController
        left_wheel: wheel_left_joint
        right_wheel: wheel_right_joint
        publish_rate: 50
        pose(twist)_covariance_diagonal: []
    
        linear:
            x:
                has_velocity_limits:     true
                has_acceleration_limits:   false
                has_jerk_limits:         false
                max_velocity:            3.0

                min_velocity:            -3.0
                max_acceleration:        0.02
                min_acceleration:        -0.02
                max_jerk:                1.0
                min_jerk:                1.0
        angular:
            z:
                has_velocity_limits:     false
                has_acceleration_limits: false
                has_jerk_limits:         false
                max_velocity:            3.0
                min_velocity:            -3.0
                max_acceleration:        0.02
                min_acceleration:        -0.02
                max_jerk:                1.0
                min_jerk:                1.0

        wheel_separation: 
        wheel_radius:
        wheel_separation_multiplier: 
        wheel_radius_multiplier:
        cmd_vel_timeout:
        base_frame_id:
        enable_odom_tf:

```

joint_state_publisher.yaml部分
```
base:
    joint_state_controller:
        type: joint_state_controller/JointStateController
        publish_rate: 50
    wheel_left:
        velocity_controller:
            type: velocity_controllers/JointVelocityController
            joint: wheel_left_joint
            pid: {p: 10.0, i: 0.01, d:0.0}
    wheel_right:(省略)
```
ros_controlの設定
```
base:
    base_driver:
        Kp: 0.04
        Ki: 0.004
        Kd: 0.1
        duty_multipier: 1.0
    略
```
---
[トップへ](#4_explorer.md)

<!--
```
プログラムを書く
```
-->