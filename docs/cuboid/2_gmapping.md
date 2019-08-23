# 2_gmmaping.md

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

## 起動コマンド

```
roslaunch cube_navigation gmapping.launch scan:=map_scan
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