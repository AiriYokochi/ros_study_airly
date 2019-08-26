# 3_navigation.md

[前記事　2_gmapping.md](2_gmapping.md)←　→[次記事 4_hoge.md](3_navigation.md)

gazeboでnavigationを行う\


## 参考リンク

## 開発環境

* Ubuntu 16.04LTS
* Linux kernel 4.15.0-58-generic
* gazebo9

## 起動コマンド

1. gazebo上にCuboidを出現させる
2. navigationを実行する
    * 引数1: map_file:=地図データを絶対パスで指定,yaml形式
    * 引数2: scan:=自己位置推定する際のレーダのトピックを指定

```
roslaunch cube_gazebo cuboid_with_house.launch
roslaunch cube_navigation navigation.launch map_file:=/home/gisen/ros/src/sbgisen/cube/cube_navigation/map/airi_room.yaml scan:=/map_scan
```

立ち入り禁止区域を設定する際
```
roslaunch cube_navigation navigation.launch map_file:=/home/gisen/ros/src/sbgisen/cube/cube_navigation/map/airi_room.yaml scan:=/map_scan map_keepout_file:=/home/gisen/ros/src/sbgisen/cube/cube_navigation/map/airi_room_keepout.yaml use_keepout:=true
```

## navigation.launchについて

### 引数

* robot:使用するロボット名
* scan::自己位置推定に用いるセンサの値を取得するトピック名
* map_file::使用する地図データ,yaml形式
* map_keepout_file::立ち入り禁止区域の地図データ
* use_keepout::立ち入り禁止を設定するか否かのフラグ


### 起動しているROSノード

* map_server::地図データをROSで使用するため
* amcl::レーザセンサを用いて自己位置推定を行う
* move_base::ゴールを設定したときにその場所まで移動する

## 立ち入り禁止区域を指定する
地図(pgm)データで、ImageMagicを用いて立ち入り禁止区域に黒線を塗った\
[立ち入り禁止区域の地図](images/airi_room_keepout.pgm)\
またnavigationを起動する際に引数としてmap_keep_fileにyamlファイルの絶対パス、とuse_keepoutにtrueを設定する。

## 結果

[navigation中のスクリーンショット](images/20190823_navigation_sc.png)

---
[トップへ](#本日やったこと)

<!--
```
プログラムを書く
```
-->