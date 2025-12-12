# ROS2 SLAM+Navigation(2/2)

## 概要

本記事では、ROS2 ( Jazzy )でのSLAM(Simultaneous Localization and Mapping)とNavigationについて調査しメモします。

## Navigation2
Nav2ではBehavior Treeを用いてロボットの行動を指定する。

Behavior Treeを実行するために4つのアクションサーバーが実装されている。

Lifescycle Manager
  - ( BT Navigatior Server )
  - Planner Server (Global Costmap)
    - 目的地までの経路を計算する
  - Contoller Server (Local Costmap)
    - 計算された経路をロボットがたどるための制御をする
  - Behavior Server (Costmap sub. Footprint Sub.)
    - 回復動作用
  - Smoother Server (Costmap sub. Footprint Sub.)
    - プランナーの経路を最適化する
  - ( Route Server )
  
コストマップ
 - static_layer : 事前に作成したマップからの情報
 - obstacle_layer : LiDARなどのセンサ情報
 - inflation_layer : 障害物付近のコストを増加させる

Navigation2ではオドメトリの誤差を吸収するためにAMCLという手法を用いる。AMCLの進化版により柔軟になったEMCLやEMCL2があり、今回はEMCL2を用いる。

## 必要な情報

## launch

- nav2_controller (nav2_controller::ControllerServer)
  - odom
  - scan
  - プランに沿って`cmd_vel`を出す
- nav2_smoother  
  - plannerが出したpathをスムーズにする 
- nav2_planner(nav2_planner::PlannerServer)
  - map + costmapから経路を作る
- nav2_behaviors(behavior_server::BehaviorServer)
    - 回復動作 (spin/backup/wait)
- nav2_bt_navigator (nav2_bt_navigator::BtNavigator)
  - odom
  - planner, controller, behaviorを順番にたたいて目的地へ
- nav2_waypoint_follower(nav2_waypoint_follower::WaypointFollower)
  - 複数地点の巡回
- nav2_lifecycle_manager
  - lifecycle_nodesのNav2群をautosutartで自動active
- nav2_map_server
  - /mapを配信
- emcl2
  - 自己位置推定、/map -> /odomを推定する
- nav2_lifecycle_manager
  - map_server, emcl
  - 上2つを自動でactiveにする
- map_server
  - 禁止領域をmap_serverで配信
- map_server(costmap_filter_info_server)
  - mask_topic
  - keepout filter が参照する「どのmaskを使うか」「係数は何か」等の情報を出す

Nav2の管理対象
```python
    lifecycle_nodes = [
        'controller_server',
        'smoother_server',
        'planner_server',
        'behavior_server',
        'bt_navigator',
        'waypoint_follower',
        'keepout_mask_server',
        'costmap_filter_info_server',
    ]
```

---

## param

#### EMCL2
##### 基本フレーム
```yaml
emcl:
  ros__parameters:
    global_frame_id: map
    odom_frame_id: odom
    base_frame_id: base_link
    footprint_frame_id: base_footprint
```
`base_footprint`を使ってるのは 段差・傾きの影響を避けるため


```yaml
    odom_freq: 20
    transform_tolerance: 0.2
```
- odom更新頻度の想定
- TF遅延の許容
- ロボットが速いほど tolerance は大きめに必要

#### LiDAR入力
```yaml
    laser_min_range: 0.0
    laser_max_range: 100000000.0
    scan_increment: 10
```
- scan_increment：何本に1本使うか
  - 10 → 計算軽い・精度は落ちる
  - 5 → 精度↑・CPU↑

#### 初期姿勢
```yaml
    initial_pose_x: 0.0
    initial_pose_y: 0.0
    initial_pose_a: 0.0
    num_particles: 500
```
- 粒子数は 安定性とCPUのトレードオフ
- 室内ロボットなら 500〜1000 が現実的

#### リサンプリング・分散制御

```yaml

    alpha_threshold: 0.5
    expansion_radius_position: 0.1
    expansion_radius_orientation: 0.2
    extraction_rate: 0.1
```
- 自己位置が怪しくなった時の“広がり方”
- 迷子になりやすいなら radius ↑
- ピタッと安定させたいなら ↓


#### オドメトリ誤差モデル（最重要）
```yaml
    odom_fw_dev_per_fw: 0.19
    odom_fw_dev_per_rot: 0.0001
    odom_rot_dev_per_fw: 0.13
    odom_rot_dev_per_rot: 0.2
```
どれだけ odom を信用しないか
数値↑ → 粒子が広がる（復帰力↑・ブレ↑）
数値↓ → 安定（でも迷子から戻れない）
床が滑る／ギアバックラッシュあるなら上げる

#### LiDAR一致度
```yaml
    range_threshold: 0.05
    sensor_reset: false
    laser_likelihood_max_dist: 0.2
```
scan と map のズレ許容
小さすぎると「常に外れ」
大きすぎると「適当でもOK」

---

### controller_server

```yaml
controller_server:
  ros__parameters:
    controller_frequency: 20.0 # = 1 / model_dt (for MPPI)
```
- controller_server が cmd_vel を出す周期 [Hz]
- CPUきつい / 通信遅い → 10Hzに落とす

```yaml
    min_x_velocity_threshold: 0.001
    min_theta_velocity_threshold: 0.001
```
- これ以下の速度は “0 とみなす” 閾値
- モータのデッドゾーン・微小指令のノイズでロボットがプ- ルプルするのを防ぐ
- 0.001 はかなり小さい（ほぼ切らない設定）
- 停止付近でプルプル → 0.02〜0.05 とかに上げる

```yaml
    failure_tolerance: 0.3
```
- 「どれくらい経路追従に失敗しても許すか」的な許容
- 小さい → すぐ失敗扱い（安全寄り、recovery多め）
- 大きい → 粘る（recovery減るが詰まると長引く）

```yaml
    enable_stamped_cmd_vel: true
```
-出力を geometry_msgs/TwistStamped にする

```yaml
    progress_checker_plugins: [progress_checker]
    goal_checker_plugins: [general_goal_checker]
    controller_plugins: [FollowPath]
    # Progress checker parameters
    progress_checker:
      plugin: nav2_controller::SimpleProgressChecker
      required_movement_radius: 0.5
      movement_time_allowance: 10.0
    # Goal checker parameters
    general_goal_checker:
      stateful: true
      plugin: nav2_controller::SimpleGoalChecker
      xy_goal_tolerance: 0.1
      yaw_goal_tolerance: 0.05
```
- 「一定時間の間に、一定距離以上進んでないなら詰まってる」
- required_movement_radius: 10秒以内に 0.5m 進めないなら「進捗なし」
- movement_time_allowance: 監視ウィンドウの時間 [s]
- required_movement_radius を 0.1〜0.3 に下げる
- もしくは movement_time_allowance を伸ばす
- xy_goal_tolerance:10cm 以内なら到達
- yaw_goal_tolerance: 約 2.9° 以内
- stateful: 一度ゴール判定に入ったら、多少ズレても “到達扱いを維持” する挙動になりやすい
- ゴール前で延々微調整 → xy を 0.15〜0.25、yaw を 0.1 くらいへ緩める

```yaml
    # MPPI parameters
    FollowPath:
      plugin: "nav2_mppi_controller::MPPIController"
      time_steps: 71
      model_dt: 0.05
```
- 予測時間 = time_steps * model_dt = 3.55 秒
- MPPIは「未来の操作列」をサンプルして、コストが小さいものを選ぶ方式
→ 予測が長いほど “先読み” するけど計算が増える＆鈍くなることもある
- 狭い場所で曲がりが遅い/モタつく → time_steps を 40〜60 に
- 先読みできず角で膨らむ → もう少し長く or critics調整

```yaml
      batch_size: 500
```
毎周期に評価するサンプル軌道の本数
大きいほど賢くなるが重い
CPUきつい → 200〜400に
動きが変/局所解にハマる → 800〜1000へ
```yaml
    　vx_std: 0.5
      wz_std: 0.4
```
サンプリングする操作列のばらつき（探索幅）
大きい＝大胆に探索（急旋回/急加速も候補に出る）
小さい＝おとなしい探索（滑らかだが詰まりやすい）
ロボが荒い・蛇行 → std を下げる
狭い場所で停滞 → std を上げる（or Costmap/Inflation見直し）

```yaml
      vx_max: 0.7
      vx_min: -0.2
      wz_max: 1.57
      ax_max: 3.0 # 0.1
      ax_min: -3.0 # -0.6
      az_max: 4.71
```
速度制限（必ず実機の限界と一致させる）
vx_min が負なので後退を許可
wz_max=1.57 は 90°/s（結構速い）
回転が怖い → wz_max を下げる（1.0とか）
後退させたくない → vx_min を 0.0 に（ただし詰まりやすくなる）
本来は加速度制約のつもりだが、コメント通り 実装上の事情がある（PR言及）
az_max は角加速度上限
ロボが急にギクッとなる → ここを下げたい気持ちは正しいが、実際に効き方が想定通りでないことがあるので、まずは
vx_std/wz_std と wz_max で丸める方が安全

```yaml
      iteration_count: 1
```
1周期あたりの最適化反復回数
1は軽い（リアルタイム重視）
上げると賢くなるが重い
```yaml
      prune_distance: 2.485 # prediction_horizon
```
ゴールが近い時などに、先のパスを切り詰める距離
先読みしすぎて遠回り/膨らむ → 少し短く
ゴール前でバタつく → 少し長くもあり（ただし他の要因が多い）
```yaml
      transform_tolerance: 0.2
```
TFの遅延許容 [s]
TF遅延が大きいシステムだとこれで落ちる
TF extrapolation error が出る → 0.3〜0.5に上げる
```yaml
      temperature: 0.3
```
MPPIの “ソフトマックス” 的な重み付けの鋭さ（ざっくり）
小さい → 良い軌道に強く寄る（鋭い、局所解寄り）
大きい → 平均化される（穏やか、鈍い）
ふらつき/ランダム感 → 下げる方向
局所解にハマる/抜けない → 上げる方向
```yaml
      gamma: 0.015
```
コストに対する “割引” や安定化に効く係数（MPPIの実装依存）
ここは触る優先度は低め
→ 触るなら小さく刻む（0.01〜0.03程度の範囲で）
```yaml
      motion_model: "DiffDrive"
```
差動二輪用の運動モデル
```yaml
      visualize: false
      regenerate_noises: true
      TrajectoryVisualizer:
        trajectory_step: 10
        time_step: 5
```
visualize true にすると可視化用トピックが増えて重くなるがデバッグに便利
regenerate_noises: true は探索ノイズを毎回更新（局所停滞しにくい）
Visualizerの step は “どれくらい間引いて表示するか” で挙動には本質影響なし
```yaml
      ConstraintCritic:
        enabled: true
        cost_power: 1
        cost_weight: 4.0
```
障害物に寄りすぎる → cost_weight 上げる or inflation見直し
進まない（過敏）→ cost_weight 下げる or costmapが厳しすぎ
```yaml
    GoalCritic:
        enabled: true
        cost_power: 1
        cost_weight: 50.0
        threshold_to_consider: 1.0 # adjust for overshooting, The same value as prediction_horizon(2.485) is recommended
```
ゴールでオーバーシュート → threshold や weight 調整
ゴールを無視して避け続ける → weight 上げる
```yaml
      GoalAngleCritic:
        enabled: true
        cost_power: 1
        cost_weight: 30.0
        threshold_to_consider: 0.1
```
ゴール向き（姿勢）を合わせる
threshold 小さいので かなりゴール直前だけ効く
ゴール付近でくるくる回る → weight 下げる / yaw_goal_tolerance 緩める
```yaml
      PreferForwardCritic:
        enabled: true
        cost_power: 1
        cost_weight: 100.0
        threshold_to_consider: 0.5
```
後退を嫌う（＝前進を優先）
狭所で後退が必要なときに詰まりやすくなる原因にもなる
狭いところで詰まる → weight を下げる（30〜70へ）か、vx_min を 0 にしない運用にする
```yaml
      CostCritic:
        enabled: true
        cost_power: 1
        cost_weight: 3.81
        critical_cost: 300.0
        consider_footprint: true
        collision_cost: 1000000.0
        near_goal_distance: 1.0
        trajectory_point_step: 2
```

```yaml
      PathAlignCritic:
        enabled: true
        cost_power: 1
        cost_weight: 25.0
        max_path_occupancy_ratio: 0.05
        trajectory_point_step: 4
        offset_from_furthest: 24 # = prediction_time * max_speed / path_resolution / 3.0 = approx 23.66
        threshold_to_consider: 1.0 # adjust for overshooting, The same value as prediction_horizon(2.485) is recommended
        use_path_orientations: false
```
パスに “整列” する（横ズレや向き）
offset_from_furthest は「パスのどのあたりを見て整列するか」
→ 先の方（大きい）を見ると先読み、手前だと即時反応
max_path_occupancy_ratio：パス上が障害物で塞がれ気味なら、このcriticを効かせない等の抑制
パスから離れがち → weight 上げる / offset 調整
先読みしすぎて膨らむ → offset を下げる

```yaml
      PathFollowCritic:
        enabled: true
        cost_power: 1
        cost_weight: 5.0
        offset_from_furthest: 5
        threshold_to_consider: 1.0 # adjust for overshooting, The same value as prediction_horizon(2.485) is recommended
```
こちらは “追従” 寄り（PathAlignより弱い）
offset 小さいので手元寄りを見てる

```yaml
      PathAngleCritic:
        enabled: true
        cost_power: 1
        cost_weight: 2.0
        offset_from_furthest: 4
        threshold_to_consider: 1.0 # adjust for overshooting, The same value as prediction_horizon(2.485) is recommended
        max_angle_to_furthest: 1.0
        mode: 0
```
パスの進行方向に頭を向ける
max_angle_to_furthest は “先の点を見るときの角度制限” みたいな安全弁
mode は実装依存の分岐（通常触らない）

---

1) うねる・蛇行する
- vx_std ↓、wz_std ↓
- wz_max ↓
- （次）PathAlignCritic cost_weight ↑

2) 障害物を怖がって進まない
- local_costmap inflation を弱める（半径/係数）
- CostCritic cost_weight ↓
- batch_size ↑（候補が足りないと停滞しやすい）

3) 狭いところで詰まる
- PreferForwardCritic cost_weight ↓
- vx_min をもう少し負に（-0.25 など）も検討
- time_steps ↓（短くしてキビキビ）

4) ゴール付近でぐるぐる / 微振動
- yaw_goal_tolerance 緩める（0.1）
- GoalAngleCritic cost_weight ↓
- min_theta_velocity_threshold ↑

---

```yaml
local_costmap:
  local_costmap:
    ros__parameters:
      update_frequency: 10.0
      publish_frequency: 10.0
      global_frame: odom
      robot_base_frame: base_footprint
      rolling_window: true
      width: 5 # 2.5cm
      height: 5 #2.5cm
      resolution: 0.01 # 1cm grid 軽量化したい → resolution を 0.02 に（最初に下げるならここ）
      # yamllint disable-line rule:line-length
      footprint: "[[-0.11, 0.11], [0.11, 0.11], [0.11, -0.11], [-0.11, -0.11]]"
      footprint_padding: 0.05 # 机脚が多い / 狭い → 0.02〜0.03
      plugins:
        - laser_obstacles
        - stvl_layer
        - inflation_layer
      filters: [keepout_filter]
      inflation_layer:
        plugin: nav2_costmap_2d::InflationLayer
        cost_scaling_factor: 10.0 # 進まないときは0.6~0.8
        inflation_radius: 1.0 # 半径 1m 以内は徐々に危険 進まないときは0.6~0.8
        inflate_unknown: true
      laser_obstacles:
        plugin: nav2_costmap_2d::ObstacleLayer
        enabled: true
        observation_sources: laser
        combination_method: 1
        laser:
          sensor_frame: pacecat_link
          data_type: LaserScan
          topic: laser/scan
          marking: true
          clearing: true
          min_obstacle_height: 0.05
          max_obstacle_height: 1.0 # 0.6
          expected_update_rate: 0.0 # default 0, if not updating at this rate at least, remove from buffer
          observation_persistence: 0.0 # default 0, use all measurements taken during now-value, 0=latest
          inf_is_valid: true # default false, for laser scans
          nan_is_valid: true
          raytrace_max_range: 30.0
          obstacle_max_range: 27.0
      stvl_layer:
        plugin: spatio_temporal_voxel_layer/SpatioTemporalVoxelLayer
        enabled: true
        voxel_decay: 15.
        decay_model: 0
        voxel_size: 0.01
        track_unknown_space: true
        unknown_threshold: 15
        mark_threshold: 0
        update_footprint_enabled: true
        combination_method: 1
        origin_z: 0.0
        publish_voxel_map: true
        transform_tolerance: 0.2
        mapping_mode: false
        map_save_duration: 60.0
        observation_sources: head_mark head_clear
        head_mark:
          data_type: PointCloud2
          topic: camera/depth_registered/cost_points
          marking: true
          clearing: false
          obstacle_range: 2.0
          min_obstacle_height: 0.07
          max_obstacle_height: 2.0
          expected_update_rate: 0.0
          observation_persistence: 0.0
          inf_is_valid: false
          clear_after_reading: true
          filter: passthrough
        head_clear:
          data_type: PointCloud2
          topic: camera/depth_registered/cost_points
          marking: false
          clearing: true
          max_z: 7.0
          min_z: 0.0
          vertical_fov_angle: 1.1345
          horizontal_fov_angle: 1.309
          decay_acceleration: 15.0
          model_type: 0
      keepout_filter:
        plugin: nav2_costmap_2d::KeepoutFilter
        enabled: true
        filter_info_topic: costmap_filter_info
      always_send_full_costmap: true

```

```yaml
global_costmap:
  global_costmap:
    ros__parameters:
      update_frequency: 3.0
      publish_frequency: 1.0
      global_frame: map
      robot_base_frame: base_footprint
      resolution: 0.03
      track_unknown_space: true
      # yamllint disable-line rule:line-length
      footprint: "[[-0.11, 0.11], [0.11, 0.11], [0.11, -0.11], [-0.11, -0.11]]"
      footprint_padding: 0.05
      plugins:
        - static_layer
        - laser_obstacles
        - stvl_layer
        - inflation_layer
      filters: [keepout_filter]
      laser_obstacles:
        plugin: nav2_costmap_2d::ObstacleLayer
        enabled: true
        observation_sources: laser
        combination_method: 1
        laser:
          sensor_frame: pacecat_link
          data_type: LaserScan
          topic: /laser/scan
          marking: true
          clearing: true
          min_obstacle_height: 0.05
          max_obstacle_height: 1.0 # 0.6
          expected_update_rate: 0.0 # default 0, if not updating at this rate at least, remove from buffer
          observation_persistence: 0.0 # default 0, use all measurements taken during now-value, 0=latest
          inf_is_valid: true # default false, for laser scans
          nan_is_valid: true
          raytrace_max_range: 30.0
          obstacle_max_range: 27.0
      stvl_layer:
        plugin: spatio_temporal_voxel_layer/SpatioTemporalVoxelLayer
        enabled: true
        voxel_decay: 15.
        decay_model: 0
        voxel_size: 0.03
        track_unknown_space: true
        unknown_threshold: 15
        mark_threshold: 0
        update_footprint_enabled: true
        combination_method: 1
        origin_z: 0.0
        publish_voxel_map: true
        transform_tolerance: 0.2
        mapping_mode: false
        map_save_duration: 60.0
        observation_sources: head_mark head_clear
        head_mark:
          data_type: PointCloud2
          topic: /camera/depth_registered/cost_points
          marking: true
          clearing: false
          obstacle_range: 2.0
          min_obstacle_height: 0.07
          max_obstacle_height: 2.0
          expected_update_rate: 0.0
          observation_persistence: 0.0
          inf_is_valid: false
          clear_after_reading: true
          filter: passthrough
        head_clear:
          data_type: PointCloud2
          topic: /camera/depth_registered/cost_points
          marking: false
          clearing: true
          max_z: 7.0
          min_z: 0.0
          vertical_fov_angle: 1.1345
          horizontal_fov_angle: 1.309
          decay_acceleration: 15.0
          model_type: 0
      static_layer:
        plugin: nav2_costmap_2d::StaticLayer
        map_subscribe_transient_local: true
      inflation_layer:
        plugin: nav2_costmap_2d::InflationLayer
        cost_scaling_factor: 5.0
        inflation_radius: 1.0
      keepout_filter:
        plugin: nav2_costmap_2d::KeepoutFilter
        enabled: true
        filter_info_topic: costmap_filter_info
      always_send_full_costmap: true
```

```yaml
map_server:
  ros__parameters:
    frame_id: map
```

```yaml
keepout_mask_server:
  ros__parameters:
    frame_id: map
    topic_name: keepout_mask
```

```yaml
costmap_filter_info_server:
  ros__parameters:
    type: 0
    filter_info_topic: costmap_filter_info
    base: 0.0
    multiplier: 1.0
```

```yaml
map_saver:
  ros__parameters:
    save_map_timeout: 5.0
    free_thresh_default: 0.25
    occupied_thresh_default: 0.65
    map_subscribe_transient_local: true
```

```yaml
planner_server:
  ros__parameters:
    expected_planner_frequency: 20.0
    planner_plugins: [GridBased]
    GridBased:
      plugin: nav2_navfn_planner::NavfnPlanner
      tolerance: 0.0
      use_astar: false
      allow_unknown: true
```

```yaml
behavior_server:
  ros__parameters:
    enable_stamped_cmd_vel: true
    local_costmap_topic: local_costmap/costmap_raw
    global_costmap_topic: global_costmap/costmap_raw
    local_footprint_topic: local_costmap/published_footprint
    global_footprint_topic: global_costmap/published_footprint
    cycle_frequency: 10.0
    behavior_plugins: [spin, backup, wait]
    spin:
      plugin: nav2_behaviors::Spin
    backup:
      plugin: nav2_behaviors::BackUp
    wait:
      plugin: nav2_behaviors::Wait
    global_frame: odom
    robot_base_frame: base_footprint
    transform_timeout: 0.1
    simulate_ahead_time: 2.0
    max_rotational_vel: 1.0
    min_rotational_vel: 0.4
    rotational_acc_lim: 3.2
```

```yaml
waypoint_follower:
  ros__parameters:
    loop_rate: 20
    stop_on_failure: false
    waypoint_task_executor_plugin: wait_at_waypoint
    wait_at_waypoint:
      plugin: nav2_waypoint_follower::WaitAtWaypoint
      enabled: true
      waypoint_pause_duration: 200
```

```yaml
smoother_server:
  ros__parameters:
    smoother_plugins: [smooth_path]
    smooth_path:
      plugin: nav2_constrained_smoother/ConstrainedSmoother
      # whether to detect forward/reverse direction and cusps.
      # Should be set to false for paths without orientations assigned
      reversing_enabled: true
      path_downsampling_factor: 3 # every n-th node of the path is taken. Useful for speed-up
      # 0 - path remains downsampled,
      # 1 - path is upsampled back to original granularity using cubic bezier, 2... - more upsampling
      path_upsampling_factor: 1
      keep_start_orientation: true # whether to prevent the start orientation from being smoothed
      keep_goal_orientation: true # whether to prevent the gpal orientation from being smoothed
      # minimum turning radius the robot can perform. Can be set to 0.0
      # (or w_curve can be set to 0.0 with the same effect) for diff-drive/holonomic robots
      minimum_turning_radius: 0.0
      w_curve: 0.0 # weight to enforce minimum_turning_radius
      w_dist: 0.0 # weight to bind path to original as optional replacement for cost weight
      w_smooth: 15000.0 # weight to maximize smoothness of path
      w_cost: 0.015 # weight to steer robot away from collision and cost

      # Parameters used to improve obstacle avoidance near cusps (forward/reverse movement changes)
      # option to use higher weight during forward/reverse direction change
      # which is often accompanied with dangerous rotations
      w_cost_cusp_multiplier: 3.0
      # length of the section around cusp in which nodes use w_cost_cusp_multiplier
      # (w_cost rises gradually inside the zone towards the cusp point,
      # whose costmap weight eqals w_cost*w_cost_cusp_multiplier)
      cusp_zone_length: 2.5

      # Points in robot frame to grab costmap values from. Format: [x1, y1, weight1, x2, y2, weight2, ...]
      # IMPORTANT: Requires much higher number of iterations to actually improve the path.
      # Uncomment only if you really need it (highly elongated/asymmetric robots)
      # cost_check_points: [-0.185, 0.0, 1.0]

      optimizer:
        max_iterations: 70 # max iterations of smoother
        debug_optimizer: false # print debug info
        gradient_tol: 5000.0
        fn_tol: 1.0e-15
        param_tol: 1.0e-20

```