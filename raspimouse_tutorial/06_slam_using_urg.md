# Raspberry Pi Mouse Simulator's Tutorial for airy

[前回の記事 01_setup.md)](./01_setup.md)\
[前回の記事 00_Inventor.md](./00_Inventor.md)\
[前回の記事 02_kyori_sensor_test.md](./02_kyori_sensor_test.md)
[前回の記事 05_kyori_sensor_test.md](./05_kyori_sensor_test.md)
[前回の記事 05_kyori_sensor_test_Part2.md](./05_kyori_sensor_test_Part2.md)

## 内容
1. [Part6_シミュレータ上のラズパイマウスを動かす方法](#Part6_シミュレータ上のラズパイマウスを動かす方法)

### 参考
* [Raspberry Pi Mouse Simulator's Tutorial](https://raspimouse-sim-tutorial.gitbook.io/project/)
* [how_to_install_simulator_xenial](https://github.com/rt-net/raspimouse_sim/wiki/how_to_install_simulator_xenial) 
* [作者ブログ](https://products.rt-net.jp/micromouse/archives/tag/ros)
* [Github arpg/Gazebo/world/willowgarage.world](https://github.com/arpg/Gazebo/blob/master/worlds/willowgarage.world)
* [Running turtlebot simulator in willow garage world](https://answers.ros.org/question/12956/running-turtlebot-simulator-in-willow-garage-world/)


### 所感　時間がかかったこと、難しかった、やりたいことなど
[参考][HOKUYO トップページ->製品の原理 用語解説->測域センサ->測域センサ](https://www.hokuyo-aut.co.jp/products/data.php?id=41)
[所感]測域センサ使ってみたい！！！！

## Part6_シミュレータ上のラズパイマウスを動かす方法
### 概要
測域センサを用いてSLAMを行う\
コントローラを用いてラズパイマウスを動かし、地図を作成したあとに保存する

### 調査
#### 測域センサ
* 光をスキャンニングしながら検出物までの距離を測定する二次元走査型の光距離センサ
* レーザスキャナ、LRF(Laser rangefinder)、LIDER(Light Detection and Ranging)とも呼ぶ
* 所定エリア内を平面上にトレースしたデータを出力するので、エリア内の検出物の大きさ、位置、移動方向の判別が可能
    * 例えば人の大きさ、位置、移動しているときの方向
* 検出データを直接出力するシリアルタイプ
* エリア設定内の検出物の有無判定を行うパラレルタイプ、このエリア内に検出物があるか？
* センサ自身が光を出して反射光を検出するので暗闇でも使用可能
* モータで本体を回転させる方式、ミラーを動かす方式、回転物を搭載しないMEMS方式のLiDERがある
#### SLAM
* Simultaneous Localization and Mappingの略で、自己位置推定と地図生成を同時に行う
* LIDERと呼ばれるセンサを使用する
* 必要なトピックはtfとscan
    * tf:ロボットの位置姿勢や座標を変換したり配信するトピック、ロボットの移動量から座標変換したパラメータを使用する
    * scan:URGから取得できるセンサ値を含むトピック
* ROSノードはSLAMとmap=server
    * SLAM:自己位置推定と地図生成を同時に行う、地図生成はtf,scanトピックからmapデータを生成する
    * map_server:mapデータから地図画像.pgmと地図データ.yamlを生成する、mapトピックで配信する

### 起動
ファイルをいくつか書き換えたあと、以下のコマンドで起動する。\
書き換えるファイルは以下\
* 今回ラズパイマウスのデータを書き換えているので対応しているurdfを修正(bodyのイナーシャ)
* 読み込むworldファイルが無いので作成
```
roslaunch raspimouse_gazebo raspimouse_with_willowgarage.launch
```
#### 修正箇所
今回、willowgarage.worldがないので作成する。方法は２つでどっちでもいい
1. willowgarage.worldの作成
[Github](https://github.com/arpg/Gazebo/blob/master/worlds/willowgarage.world)から持ってくる
```
<?xml version="1.0" ?>
<sdf version="1.5">
  <world name="default">
    <include>
      <uri>model://ground_plane</uri>
    </include>
    <include>
      <uri>model://sun</uri>
    </include>
    <include>
      <uri>model://willowgarage</uri>
    </include>
  </world>
</sdf>
```
2. launchファイルの書き換え
    * gazeboワールドの起動部分コメント
    * gazebo/ros/launch/willowgarage_world.launchの起動
    ```
    <!-- We resume the logic in empty_world.launch, changing only the name of the world to be launched -->
    <!-- <include file="$(find gazebo_ros)/launch/empty_world.launch">
        <arg name="world_name" value="$(find raspimouse_gazebo)/worlds/willowgarage.world" />
        <arg name="debug" value="$(arg debug)" />
        <arg name="gui" value="$(arg gui)" />
        <arg name="paused" value="$(arg paused)" />
        <arg name="use_sim_time" value="$(arg use_sim_time)" />
        <arg name="headless" value="$(arg headless)" />
    </include> -->

    <include file="$(find gazebo_ros)/launch/willowgarage_world.launch"/>
    ```
今回、とりあえず2の方法ですすめる

#### SLAMを行う
PS3コントローラを用いてラズパイマウスを動かす。\
gmappingの後にJOYノードを起動するとgamappingが落ちるので注意
```
roslaunch basic_lecture turtle.launch
roslaunch raspimouse_gazebo raspimouse_with_willowgarage.launch
roslaunch raspimouse_sim_tutorial raspimouse_sim_gmapping.launch
```
turtle.launchの中身\
basic_lectureは[QIITA ROS講座07 joyプログラム](https://qiita.com/srs/items/9114bb3c27a148e0b855)を書き換えた
```
<launch>
    <node name="joy_node" pkg="joy" type="joy_node" />
    <node name="basic_twist_publisher" pkg="basic_lecture" type="basic_twist_publisher">
    <remap from="cmd_vel" to="raspimouse_on_gazebo/diff_drive_controller/cmd_vel"/>
    </node>
</launch>
```
#### マップの保存
map_serverパッケージを用いる\
セーブ用のノードmap_saverを実行する
```
roscd raspimouse_sim_tutorial_program/
rosrun map_server map_saver -f map/room
```

### 理解
[トップへ](#内容)

<!--
```
プログラムを書く
```
-->

