# Raspberry Pi Mouse Simulator's Tutorial for airy

## 参考
* [Raspberry Pi Mouse Simulator's Tutorial](https://raspimouse-sim-tutorial.gitbook.io/project/)
* [how_to_install_simulator_xenial](https://github.com/rt-net/raspimouse_sim/wiki/how_to_install_simulator_xenial) 

## 本日やったこと
1. [セットアップ](#セットアップ)
2. [実行](#実行)

## 時間
10:30-12:05::セットアップ\
13:30-13:35::セットアップ

## 次回やること
シミュレーションのチュートリアルを読む
* 迷路の表示
* 距離センサーを使ってみる

## 所感　時間がかかったこと、難しかった、やりたいことなど
<!-- 
[解決/未解決/所感/疑問/参考など]
[参考][記事名](URL)
 -->
* イナーシャの値やkpkdの値が難しい
---
---
## セットアップ
### 概要
<!-- 何をするもの、方法と結果を３行程度 -->
Raspberry Pi Mouse Simulatorのインストールをする。\
動作環境が異なるので注意
### 動作環境
* Ubuntu Desktop 16.04 LTS 64bit
    * ROS Kinetic
    * Gazebo 9.9.0
**Gazeboのバージョンが異なると物理演算に使用する係数の初期値が異なるので注意**
### その他
* catkin_make -> catkin build
* apt-get -> apt

### セットアップ手順
[手順](https://github.com/rt-net/raspimouse_sim/wiki/how_to_install_simulator_xenial)を参考に
インストーラが起動しなかったのでgitからcloneしてくる\
```
git clone https://github.com/rt-net/raspimouse_sim.git
catkin build
```
[トップへ](#本日やったこと)
---
## 実行
### 概要
gazeboを立ち上げてラズパイと迷路を表示させる\
エラーの修正をする\
### 実行方法
/devなんたらが無いエラーが出るので1行目を実行
2行目を実行するとgazeboが立ち上がって迷路とラズパイが表示されるはず！
```
rosrun raspimouse_control gen_dev_file.sh
roslaunch raspimouse_gazebo raspimouse_with_samplemaze.launch
```
表示されなかったので以下変更

#### ラズパイホイールのタイヤが表示されないエラー1_legacyModeNSの追加
xacroファイルのgazeboタグにlegacyModeNSを追加する\
* raspimouse_description/raspimouse_urdf.xacro
* raspimouse_description/robot/raspimouse_urdf.xacro
```
<gazebo>
    <plugin name="gazebo_ros_control" filename="libgazebo_ros_control.so">
        <robotNamespace>raspimouse_on_gazebo</robotNamespace>
+       <legacyModeNS>true</legacyModeNS>
    </plugin>
</gazebo>
```
#### ラズパイホイールのタイヤが表示されないエラー2_pid_gainの追加
* raspimouse_control/contoller.yaml
一番下に追記
```
/gazebo_ros_control:
  pid_gains:
    left_wheel_joint:
      p: 10.0
      i: 0.0
      d: 0.0
    right_wheel_joint:
      p: 10.0
      i: 0.0
      d: 0.0
```
#### ラズパイホイールのタイヤが飛んでいくエラー_kpkdの値設定
すべてのリンクにkp kdの設定とmu2のコメントアウトをする\
今回のリンクはボディとホイール
* raspimouse_description/urdf/body/body_urg.gazebo.xacro
* raspimouse_description/urdf/body/body.gazebo.xacro
* raspimouse_description/urdf/wheel/wheel.gazebo.xacro
```
    <mu1>0.05</mu1>
-    <!-- <mu2>0.05</mu2> -->
+    <!-- <mu2>0.05</mu2> -->
+    <kp value="2000000" />
+    <kd value="100" />
```
#### ラズパイホイールのタイヤが飛んでいくエラー_mesh変更(とりあえず)
* raspimouse_description/urdf/wheel.urdf.xacro
```
   <collision>
        <geometry>
-          <!-- <cylinder radius="${wheel_radius}" length="${wheel_length}" /> -->
-          <mesh filename="package://raspimouse_description/meshes/dae/wheel/RasPiMouse_wheel.dae" />
+          <cylinder radius="${wheel_radius}" length="${wheel_length}" />
+          <!-- <mesh filename="package://raspimouse_description/meshes/dae/wheel/RasPiMouse_wheel.dae" /> -->
        </geometry>
    </collision>
```
#### ラズパイホイールのタイヤが飛んでいくエラー_重さと慣性モーメントの値変更
* raspimouse_description/urdf/body/body_urg.urdf.xacro
* raspimouse_description/urdf/body/body.urdf.xacro
重さを30kgにしました
```
-    <mass value="0.759881" />
-    <inertia ixx="0.001561279589" ixy="0.00000150035" iyy="0.001698462955" 
              ixz="0.000042679786" iyz="0.000023629024" izz="0.000855782529" />
+    <mass value="31.20594944"/>
+        <inertia
+            ixx="1.79583172"
+            ixy="0.03278618" iyy="1.55345103"
+            ixz="0.02535482" iyz="0.01272625" izz="1.22908514"/>
+    </inertial>
```
* raspimouse_description/urdf/wheel/wheel.urdf.xacro
重さを3kgにしました
```
-    <mass value="${wheel_mass}"
-    <inertia ixx="0.000002208187" ixy="0.000000000001" iyy="0.00000218412" 
        ixz="0.000000004787" iyz="0.000000000003" izz="0.000004078299" />
+    <mass value="3.16354494"/>
+    <inertia
            ixx="0.02296002"
            ixy="0.00000000" iyy="0.04452360"
            ixz="0.00000000" iyz="-0.00000001" izz="0.02296002"/>
```

[トップへ](#本日やったこと)

<!--
```
プログラムを書く
```
-->