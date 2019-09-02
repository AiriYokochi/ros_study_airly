# 4_explorer.md

[前記事　3_navigation.md](3_navigation.md)←　→[次記事 5_costmap_common_yaml.md](5_costmap_common_yaml.md)

explorerを行う

## 参考リンク

## 開発環境

* Ubuntu 16.04LTS
* Linux kernel 4.15.0-58-generic
* gazebo9

## 起動コマンド

1. gazebo上にCuboidを出現させる
2. explorer_liteを起動させる

```
roslaunch cube_gazebo cuboid_with_house.launch
roslaunch cube_navigation explorer_lite.launch scan:=map_scan robot:=cuboid
```

## explorer_lite.launchについて

### 引数

* robot:使用するロボット名
* scan::自己位置推定に用いるセンサの値を取得するトピック名
* move_base_include::move_baseを起動するlaunchファイルのパス

### explore_liteの引数

* robot_base_link::base_ling
* costmap_topic::map
* costmap_updates_topic::map_updates

### 起動しているROSノード

* gmapping::SLAMを行う
* move_base::explorerにはmove_baseを起動する必要がある。ゴールを設定したときにその場所まで移動する
* explore_lite::地図の中で未知の場所を探索する

## explorer.launchについて
rviz上で値を設定してもめっちゃエラー出てる
```
[ERROR] [1567143510.354034402, 878.810000000]: Exception thrown while processing service call: Lookup would require extrapolation into the past.  Requested time 818.950000000 but the earliest data is at time 868.800000000, when looking up transform from frame [base_link] to frame [map]
[ERROR] [1567143510.354332486, 878.810000000]: Service call failed: service [/explore_server/explore_costmap/explore_boundary/update_boundary_polygon] responded with an error: Lookup would require extrapolation into the past.  Requested time 818.950000000 but the earliest data is at time 868.800000000, when looking up transform from frame [base_link] to frame [map]
[ERROR] [1567143510.354392241, 878.810000000]: Failed to set region boundary
```
```
[INFO] [1567144793.453998, 638.210000]: [rotate_in_place] Rotate here.
update frame 3611
update ld=0.187971 ad=0.27508
Laser Pose= 0.258988 0.102997 0.670823
m_count 2
Average Scan Matching Score=925.441
neff= 30
R        egistering Scans:Done
update frame 3612
update ld=0.0551046 ad=0.352534
Laser Pose= 0.282261 0.0530482 0.318289
m_count 3
[ WARN] [1567144795.249921149, 640.000000000]: Illegal bounds change, was [tl: (-99.975000, -99.975000), br: (100.025003, 100.025003)], but is now [tl: (-100.000000, -100.000000), br: (99.975003, 99.975003)]. The offending layer is explore_costmap/explore_boundary
[ WARN] [1567144795.539913967, 640.290000000]: Car Maneuver recovery behavior started.
[ INFO] [1567144795.540112128, 640.290000000]: Front side cost: 253
[ INFO] [1567144795.540151189, 640.290000000]: Rear side cost: 214
[ INFO] [1567144795.540181662, 640.290000000]: Left side cost: 253
[ INFO] [1567144795.540228410, 640.290000000]: Right side cost: 209
[FATAL] [1567144795.540257831, 640.290000000]: Unable to recover!
[ WARN] [1567144795.540304850, 640.290000000]: Car maneuver recovery behavior finished.
[ WARN] [1567144795.985487601, 640.730000000]: Finished exploring room
[WARN] [1567144796.100716, 640.840000]: [rotate_in_place] move_base task is cancelled.
Average Scan Matching Score=1195.31
neff= 30
Registering Scans:Done
update frame 3613
update ld=0.0848391 ad=0.526762
Laser Pose= 0.284031 -0.0317724 -0.208473
m_count 4
[ WARN] [1567144796.316297533, 641.060000000]: No range readings received for 254.10 seconds, while expected at least every 2.00 seconds.
Average Scan Matching Score=1128.05
neff= 30
Registering Scans:Done
update frame 3707
update ld=0.0789572 ad=0.250357
Laser Pose= 0.284325 -0.108578 -0.45883
m_count 5
Average Scan Matching Score=1167.95
neff= 30
Registering Scans:Done
[ WARN] [1567144804.680476868, 649.400000000]: Illegal bounds change, was [tl: (-99.975000, -99.975000), br: (100.025003, 100.025003)], but is now [tl: (-100.000000, -100.000000), br: (99.975003, 99.975003)]. The offending layer is explore_costmap/explore_boundary
```

## 結果

---
[トップへ](#4_explorer.md)

<!--
```
プログラムを書く
```
-->