# Raspberry Pi Mouse Simulator's Tutorial for airy

[前回の記事](./01_setup.md) 

## 本日やったこと
1. [迷路の表示](#迷路の表示)
2. [実行](#実行)

### 参考
* [Raspberry Pi Mouse Simulator's Tutorial](https://raspimouse-sim-tutorial.gitbook.io/project/)
* [how_to_install_simulator_xenial](https://github.com/rt-net/raspimouse_sim/wiki/how_to_install_simulator_xenial) 
* [作者ブログ](https://products.rt-net.jp/micromouse/archives/tag/ros)
### 時間
10:30-12:05::セットアップ\
13:30-13:35::セットアップ

### 次回やること
シミュレーションのチュートリアルを読む
* 迷路の表示
* 距離センサーを使ってみる

### 所感　時間がかかったこと、難しかった、やりたいことなど
<!-- 
[解決/未解決/所感/疑問/参考など]
[参考][記事名](URL)
 -->
[調べる] xacroからworldに変換する方法について、xacroのオプションについて

---
## 迷路の表示
### 概要
<!-- 何をするもの、方法と結果を３行程度 -->
gazeboで迷路とラズパイを表示する時に、壁がひとつしか表示されないので修正。\
PCを再起動するたびにraspimouse_sim/raspimouse_control/scripts/gen_dev_file.shの実行が必要\

### 手順
壁がひとつしか表示されないのは、壁のリンク名がすべて同じWallなため。\
よって壁の一つ一つにWall1,Wall2,...と名前をつけていく。\
worldファイルはxacroファイルを作成し変換して作成されるので、xacroファイルを編集する\
編集するxacroファイルは、迷路を生成するsample_maze.world.xacroと迷路ひとつの壁を生成するsample_maze.wall.xacro

#### /raspimouse_gazebo/materials/sample_maze.world.xacroの変更
* ground_planeのパスが見つからないエラーが出るのでコメント、変換後に手動修正する
```
-  <xacro:include>
-    <uri>model://ground_plane</uri>
-  </xacro:include>
```

* 壁を呼ぶときにname属性を追加した
```
<!-- horizontal -->
-  <xacro:wall>
-    <pose>-0.090 0.006 0.025 0 0 1.57</pose>
-  </xacro:wall>
-  <xacro:wall>
-    <pose>-0.090 -0.174 0.025 0 0 1.57</pose>
-  </xacro:wall>
...
+  <xacro:wall pose="-0.090 0.006 0.025 0 0 1.57" name="wall1"/>
+  <xacro:wall pose="-0.090 -0.174 0.025 0 0 1.57" name="wall2"/>
....
```
#### /raspimouse_gazebo/materials/sample_maze.wall.xacroの変更
* 引数にnameの追加
* poseの書き方を変更
```
-  <xacro:macro name="wall" params="*pose">
-    <model name='wall'>
-      <xacro:insert_block name="pose" />

+  <xacro:macro name="wall" params="pose name">
+    <model name="${name}">
+      <!-- <xacro:insert_block name="pose" /> -->
+      <pose>${pose}</pose>
```
#### xacro->worldファイル変換するシェルスクリプトの変更
シェルスクリプト(/raspimouse_gazebo/materials/gen_maze.sh)実行する\
ワーキングディレクトリのPATH変更とinorder, oオプションの追加を行った
```
- WORKINGDIR="$HOME/catkin_ws/src/raspimouse_sim/raspimouse_gazebo"
- rosrun xacro xacro.py $WORKINGDIR/materials/sample_maze.world.xacro > $WORKINGDIR/worlds/sample_maze.world
+ WORKINGDIR="$HOME/ros/src/raspimouse/raspimouse_sim/raspimouse_gazebo"
+ rosrun xacro xacro --inorder -o $WORKINGDIR/worlds/sample_maze.world $WORKINGDIR/materials/sample_maze.world.xacro 

```

#### /raspimouse_gazebo/worlds/sample_maze.worldの変更
```
+    <include>
+    <uri>model://ground_plane</uri>
+    </include>
```
### 実行
動作確認完了したので、次回距離センサーを使ってみる
```
roslaunch raspimouse_gazebo raspimouse_with_samplemaze.launch
```
[トップへ](#本日やったこと)

---
## 実行
### 概要
gazeboを立ち上げてラズパイと迷路を表示させる\
エラーの修正をする\
### 実行方法



[トップへ](#本日やったこと)

<!--
```
プログラムを書く
```
-->