# 6_bringup_camera.md

[前記事　5_costmap_common.yaml.md](5_costmap_common.yaml.md)←　→[次記事 7_hoge.md](7_bringup_motor.md)

cuboidでcameraを起動する際の挙動などのメモ

## 参考リンク

* [ROS Kineticで複数カメラノードの起動](https://qiita.com/proton_lattice/items/c086bbce7dcc004e11c7)
* [ROS講座84 Nodeletを使う](https://qiita.com/srs/items/2718feee04643b2c8afa)

## 開発環境

* Ubuntu 16.04LTS
* Linux kernel 4.15.0-58-generic

## Nodeletとは

## camera.launchの中身

立ち上げているNodeは2つ

1. nodeletのmanager、名前はcamera_process
2. nodeletのuvcamera

以下書き方
```
 <node pkg="nodelet" type="nodelet" name="camera_process" args="manager" output="screen"/>
 <node pkg="nodelet" type="nodelet" name="uvc_camera" args="load uvc_camera/CameraNodelet /camera_process" output="screen">
 ...
 </node>
```

## uvc_cameraパッケージ

カメラを起動するROSパッケージは複数あり、他にusb_camがあるが、
MPEGなど[usb_camだと起動しないフォーマット形式がある](https://github.com/Nishida-Lab/rosbook_pkgs/issues/53)らしく(
[参考：ROS ANSWERES Garbled image problem on usb_cam](https://answers.ros.org/question/217382/garbled-image-problem-on-usb_cam/)
)uvc_cameraパッケージを使った方が良い。

### launchファイルで設定するパラメータ

* width:画像の幅(int)640
* height:画像の高さ(int)480
* fps:1秒間のフレーム数(int)30
* frame:(string)wide_stereo
* frame_id:(string)usb_cam_optical_frame
* auto_focus:オートフォーカス(bool)False
* focus_absolute:(int)0
* device:カメラのデバイス名(string)/dev/video0
* camera_info_url:そんなものはない(string)file://$(find uvc_camera)/config/ost.yaml

### remapするパラメータ

* /camera_info->/uvc/canmera_info
* /image_raw->/uvc/image_raw

---
[トップへ](#4_explorer.md)

<!--
```
プログラムを書く
```
-->