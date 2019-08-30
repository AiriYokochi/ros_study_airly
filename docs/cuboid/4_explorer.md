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

## explorer.launchについて

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

## 結果

---
[トップへ](#4_explorer.md)

<!--
```
プログラムを書く
```
-->