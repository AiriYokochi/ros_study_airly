# 2_gmmaping.md

[前記事　1_gazebo.md](1_gazebo.md)←　→[次記事 3_navigation.md](3_navigation.md)

gazeboで地図作成を行う\
[前記事　1_gazebo.md](1_gazebo.md)で作成したワールドファイルを用いた。

## 参考リンク

## 開発環境

* Ubuntu 16.04LTS
* Linux kernel 4.15.0-58-generic
* gazebo9

## 起動コマンド

```
roslaunch cube_gazebo cuboid_with_house.launch
roslaunch cube_navigation gmapping.launch scan:=map_scan
roslaunch basic_lecture joystick.launch(自作。cube_teleopのコメント外してやったほうが良い)
```

## 結果

[自室を想定したスペース](images/airi_room.pgm)を地図作成した\
[rqt_graphの結果](images/cube_gmapping_rqt_graph.png)


[トップへ](#本日やったこと)

<!--
```
プログラムを書く
```
-->