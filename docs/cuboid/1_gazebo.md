# 1_gazebo.md

[前記事　0_hoge.md](0_hoge.md)←　→[次記事](2_gmapping.md)

gazeboでロボットを表示する\
自室を想定したワールドファイルを自作した\

## 参考リンク

* nodelet[ROS nodeletの使い方](http://cryborg.hatenablog.com/entry/2016/09/19/154712)
    ROSノードのpub/sub通信ではメモリ空間を共有できないので、受け渡すデータにコピーが生じる\
    -> 画像などの大きいデータを送信する、小型ロボットに載せる等でリソースが足りなくなる\
    -> 通常のnodeと同じpub/subの仕組みをプロセス間(ノード間)ではなく同プロセス内のスレッド間でやり取りする\
    使い方は、nodelete managerというノードに複数のnode letクラスをloadしていく\
    manager起動コマンドは```rosrun nodelet nodelet manager <manager_name>```\
    classのロードコマンドは```rosrun nodelet nodelet load <pkg_name>/<nodelet_class_name> <manager_name>```\
    共有ライブラリを作成する\
    shared ptr参照可能\
    nodeletを普通のROSノードとして起動するコマンド```rosrun nodelet nodelet standalone <pkg_name>/<nodelet_class_name>```\

## 開発環境

* Ubuntu 16.04LTS
* Linux kernel 4.15.0-58-generic
* gazebo9

## セットアップ

リポジトリからcloneしセットアップスクリプトを走らせる

## gazeboでcuboidを表示させる方法

引数名robotにcuboidと指定する。\
実際にスポーンしているのはcuboid.launch.xml

## 起動コマンド
```
roslaunch cube_gazebo cuboid_with_house.launch
```

## cuboid.launch.xml

### ros_param

* robot_description::xacro指定

### 起動しているノード

* gazebo_ros/spawn_model
    gazeboワールド上にcuboidを出現させる
* joint_state_publisher/joint_state_publisher
    jointの角度を/joint_statesにpublishする(QIITA40)
* robot_state_publisher/robot_state_publisher
    staticなtfを出す
* nodelet/laserscan_nodelet_manager
    (QIITA84)
* nodelet/depthimage_to_laserscan

### 起動しているノード(別launchファイル)

* laser_filter(QIITA93)
    2D LiDARから受け取る全データのうち、ロボット自身にあたっている部分を除去する
* kinect2_with_pcl
    KinectV2から値を受け取り点群データで表示？
* picoflexx_with_pcl
    深度カメラから値を受け取り点群データで表示？
* teleop
    joystickかkeyboard入力でcuboidを動かす
* kit_control_gazebo
    ？？
* diagnostic
    ハードウェアの状況を検知して/diagnosticsにpublishする\
    OK,WARN,ERROR,STALE(メッセージが届かない)状態で管理する
* server
    robot_pose_publisherでTFを使用してロボットの位置を地図にpublish
        /base_linkと/mapフレーム間の変換
    rosbridge_websocketの起動

## cube_with_house.launch

ar.launchを元に部屋を表示させるlaunchファイルを作成
```xml
<?xml version="1.0"?>
<launch>
  <include file="$(find gazebo_ros)/launch/empty_world.launch">
    ...
+    <arg name="world_name" value="$(find cube_gazebo)/worlds/cube_with_house.world"/>
  </include>
</launch>
```

## cube_with_house.world

ar.worldを元に部屋を表示させるlaunchファイルを作成
```xml
<?xml version="1.0" ?>
<sdf version="1.4">
	<world name="default">
        ...
+		<include>
+			<uri>model://house</uri>
+		</include>
	</world>
</sdf>
```

## model

モデル作成はMetaseq3.1を使用した。\

1. モデルを作成し、mqo形式で保存(Windows)
2. フリーソフト3D Aceを用いてstlファイルに変換(Windows)
3. blenderでUV展開を行いobj形式に変換(Ubuntu)
UV展開は[QIITA記事　Gazebo + ROS で自分だけのロボットをつくる 3.COLLADAファイルをつくる](https://qiita.com/RyodoTanaka/items/b1ebd48da81669db7409)を参考にした。

### model.sdf

inertial属性の値は適当。\
90度回転して表示されたのでposeのrollに1.57radを記述した
```xml
<?xml version='1.0'?>
<sdf version="1.4">
<model name="house">
    <link name="link">
      <inertial>
        <mass>100.0</mass>
        <inertia>
          <ixx>0.083</ixx>
          <ixy>0.0</ixy>
          <ixz>0.0</ixz>
          <iyy>0.083</iyy>
          <iyz>0.0</iyz>
          <izz>0.083</izz>
        </inertia>
      </inertial>
      <collision name="collision">
        <pose>-1.5 0 0 1.57 0 0</pose>
        <geometry>
          <mesh>
            <uri>model://house/bighouse.obj</uri>
          </mesh>
        </geometry>
      </collision>
      <visual name="visual">
        <pose>-1.5 0 0 1.57 0 0</pose>
        <geometry>
          <mesh>
            <uri>model://house/bighouse.obj</uri>
          </mesh>
        </geometry>
      </visual>
    </link>
    <static>1</static>
  </model>
  <!-- TODO　SDF　IS＿STATIC　gazebo タグをつける -->
</sdf>
```

### model.config

name属性だけhouseに
```xml
<?xml version="1.0" ?>
<model>
    <name>house</name>
    <version>1.0</version>
    <sdf version="1.6">model.sdf</sdf>
    <author>
        <name></name>
        <email></email>
    </author>
    <description></description>
</model>
```

## その他

## 時間

## メモ

## 明日やること

## 全体の疑問点


### 参考


---
---

[トップへ](#本日やったこと)

<!--
```
プログラムを書く
```
-->