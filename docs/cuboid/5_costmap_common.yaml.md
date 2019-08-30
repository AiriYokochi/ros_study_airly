# 5_costmap_common.yaml.md

[前記事　4_explorer.md](4_exprlorer.md)←　→[次記事 6_hoge.md](5_costmap_common.yaml.md)

cuboidでexploration.launchを起動する際の

## 参考リンク

## 開発環境

* Ubuntu 16.04LTS
* Linux kernel 4.15.0-58-generic
* gazebo9

## costmap_common.yamlについて

* footprint::ロボットの形状。一辺が0.4[m]

* obstacles:障害物検知に使用する2Dセンサ
    1. kinect::前方にある
    2. foot_scan::
    3. body_scan::
    4. head_scan::
    5. map_scan_front::
    6. map_scan_rear::

    * obstacle_range::2.0(default 2.5)
        ロボットとの距離が2.0[m]以下のオブジェクトは障害物としてみなしコストマップに反映する
    * raytrace_rance::2.0(default 3.0)
        ロボットとの距離が2.0[m]以下のオブジェクトが検知された場合そのオブジェクトの内側の障害物がクリアされる？？
    * data_type::LaserScan, PointCloud, PointCloud2のいづれかが指定できる
    * clearing::センサデータを障害物のクリアに使うか、レーザで距離が計測された時、その内部の領域は障害物がない
    * marking::このセンサデータを障害物としてコストマップに反映させるか

* rgbd_obstacles::障害物検知に使用する2Dセンサ(高さ有り)

* laser_obstacles:::障害物検知に使用する2Dセンサ
    1. map_front_mark
    2. map_rear_mark
    

---
[トップへ](#4_explorer.md)

<!--
```
プログラムを書く
```
-->