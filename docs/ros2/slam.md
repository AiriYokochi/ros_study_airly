# ROS2 SLAM+Navigation(1/2)

## 概要

本記事では、ROS2 ( Jazzy )でのSLAM(Simultaneous Localization and Mapping)とNavigationについて調査しメモします。

## 環境

- Ubuntu24.04
- ROS2 Jazzy
- [sbgisen/cube_petit_ros](https://github.com/sbgisen/cube_petit_ros)
- gz_simでteleopができるxacro

## 環境地図の作成について

Nav1では`gmapping`を用いていたものをNav2では`slam_toolbox`というROS packages使っていきます。

- https://github.com/SteveMacenski/slam_toolbox

`slam_toolbox`ではオドメトリ(タイヤの回転数)とLiDARセンサのdataを取得し、自己位置推定をしながらマップを作成していきます。

**Subscribe topic**
- `/scan`
- `tf`

**Publish topic(主なもののみ)**
- `map`
- `pose`

### xacroの設定
- base_footprint -> base_linkの設定
[Question] base_footprintの意味は？
- lidarのプラグイン

```xacro
  <link name="base_footprint"/>
  <joint name="base_joint" type="fixed">
    <parent link="base_footprint"/>
    <child link="base_link"/>
    <origin xyz="0.0 0.0 0.0" rpy="0.0 0.0 0.0"/>
  </joint>
  <link name="base_link"/>
  <joint name="body_joint" type="fixed">
    <parent link="base_link"/>
    <child link="body_link"/>
    <origin rpy="0 0 0" xyz="0 0 0"/>
  </joint>
```

```xacro
  <gazebo reference="pacecat_link">
    <mu1>0.2</mu1>
    <mu2>0.2</mu2>
    <self_collide>false</self_collide>
    <sensor type="gpu_lidar" name="pacecat_ray">
      <pose>0 0 0 0 0 0</pose>
      <always_on>true</always_on>
      <visualize>false</visualize>
      <update_rate>5</update_rate>
      <topic>/laser/scan</topic>
      <gz_frame_id>pacecat_link</gz_frame_id>
      <lidar>
        <scan>
          <horizontal>
            <samples>400</samples>
            <resolution>1</resolution>
            <min_angle>${radians(-180)}</min_angle>
            <max_angle>${radians( 180)}</max_angle>
          </horizontal>
        </scan>
        <range>
          <min>0.16</min>
          <max>20.0</max>
          <resolution>0.01</resolution>
        </range>
        <noise>
          <type>gaussian</type>
          <mean>0.0</mean>
          <stddev>0.01</stddev>
        </noise>
      </lidar>
    </sensor>
  </gazebo>
```

```xacro
  <gazebo>
    <plugin filename="gz_ros2_control-system" name="gz_ros2_control::GazeboSimROS2ControlPlugin">
      <robot_param>robot_description</robot_param>
      <robot_param_node>robot_state_publisher</robot_param_node>
      <parameters>$(find cube_petit_gazebo)/config/base_control_gazebo.yaml</parameters>
    </plugin>
    <plugin name="gz::sim::systems::Sensors" filename="gz-sim-sensors-system">
      <render_engine>ogre2</render_engine>
    </plugin>
  </gazebo>
```

### Parameterの設定
説明する部分だけ抽出します

tfのframe名を書きます
```yaml
slam_toolbox:
  ros__parameters:
    # ROS Parameters
    odom_frame: odom
    map_frame: map
    base_frame: base_link   #default base_footprint
    scan_topic: /laser/scan
    mode: mapping # localization
```


```yaml
    debug_logging: false
    throttle_scans: 1
    transform_publish_period: 0.02 # if 0 never publishes odometry
    map_update_interval: 5.0
    resolution: 0.05
    max_laser_range: 19.8 # for rastering images
    minimum_time_interval: 0.5
    transform_timeout: 0.2
    tf_buffer_duration: 30.
    stack_size_to_use: 40000000 # program needs a larger stack size to serialize large maps
    enable_interactive_mode: true
```
`transform_timeout`: TFが利用可能になるまで待機する時間


```yaml
    # General Parameters
    use_scan_matching: true
    use_scan_barycenter: true
    minimum_travel_distance: 0.5
    minimum_travel_heading: 0.5
    scan_buffer_size: 10
    scan_buffer_maximum_scan_distance: 10.0
    link_match_minimum_response_fine: 0.1
    link_scan_maximum_distance: 1.5
    loop_search_maximum_distance: 3.0
    do_loop_closing: true
    loop_match_minimum_chain_size: 10
    loop_match_maximum_variance_coarse: 3.0
    loop_match_minimum_response_coarse: 0.35
    loop_match_minimum_response_fine: 0.45

    # Correlation Parameters - Correlation Parameters
    correlation_search_space_dimension: 0.5
    correlation_search_space_resolution: 0.01
    correlation_search_space_smear_deviation: 0.1

    # Correlation Parameters - Loop Closure Parameters
    loop_search_space_dimension: 8.0
    loop_search_space_resolution: 0.05
    loop_search_space_smear_deviation: 0.03

    # Scan Matcher Parameters
    distance_variance_penalty: 0.5
    angle_variance_penalty: 1.0

    fine_search_angle_offset: 0.00349
    coarse_search_angle_offset: 0.349
    coarse_angle_resolution: 0.0349
    minimum_angle_penalty: 0.9
    minimum_distance_penalty: 0.5
    use_response_expansion: true
```


### 地図保存
`slam_toolbox`にService callもあるが`nav2_map_server`という簡単にできるPackageがある

`ros2 run nav2_map_server map_saver_cli -t /cube_petit/navigation/map -f sample`


## 参考
https://qiita.com/kccs_mitsuhiro-teraoka/items/253623296504a95fab88

