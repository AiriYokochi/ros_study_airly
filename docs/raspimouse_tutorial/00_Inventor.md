# InventorでraspberryPiMouse[#InventorでraspberryPiMouse]
## 本日やったこと
1. InventorでraspberryPiMouse[#InventorでraspberryPiMouse]

## 時間
11:45-12:00::InventorでraspberryPiMouse\
13:15-19:15::InventorでraspberryPiMouse

## 明日やること
* タイヤ修正を反映
* イナーシャと重さの書き換え
* 画像の挿入
## 全体の疑問点


## InventorでraspberryPiMouse
### 概要
<!-- 何をするもの、方法と結果を３行程度 -->
Inventor(CADソフト)を使ってラズパイマウスのobj,mtlファイルを作成する\
その後、作成したobjファイルを使ってgazebo上に表示する\
InventorはWindowsかMacOSでしか使えないので注意\
Inventorのチュートリアル動画を見るとわかりやすい(1分40秒くらい)\

### ファイルの拡張子について
今回出てきた拡張子は以下\
#### Inventor用独自ファイル形式
* .ipt::パーツに使用する
* .iam::アセンブリ(パーツが組み合わさったもの)
#### その他
ポリゴンは形状のデータとテクスチャのデータからなる。どの拡張子が何を保存しているのか、何に対応しているのかぐぐる

* .igs(iges)::Initial Graphics Excange Specification・異なるCADシステムの間で図面データなどを伝達する標準インターフェース

* .obj::mtlファイルとセット。物体の形状を保存
* .mtl::objファイルとセット。マテリアルのテクスチャデータ
仕様
```
newmtl マテリアル名
map_Kd テクスチャファイル名
Ka RED GREEN BLUE（環境光データ。0.0～1.0の範囲）
Kd RED GREEN BLUE（拡散光データ。0.0～1.0の範囲）
Ks RED GREEN BLUE（鏡面光データ。0.0～1.0の範囲）
Ns SHININESS（鏡面反射角度。0.0～128.0の範囲）
```
* .dae::COLLAborative Design Activity
* .fbx::Alias社(Autodeskが買収)がオープンソースとして策定したファイル形式
* .stl::Standard Triangulated Language, 形状のみ？
[3Dファイルフォーマットの種類](http://www.slis.tsukuba.ac.jp/~fujisawa.makoto.fu/cgi-bin/wiki/index.php?3D%A5%D5%A5%A1%A5%A4%A5%EB%A5%D5%A5%A9%A1%BC%A5%DE%A5%C3%A5%C8%A4%CE%BC%EF%CE%E0)


### Inventorの表示単位設定
ROSの単位系に合わせる\
画面上タブの[ツール]->[ドキュメントの設定]->[単位]タブを選択
```
長さ::meter
角度::radian
時間::second
質量::kilogram
長さ寸法の表示制度::8.12345678
```
適用して閉じる

### Inventorの操作方法
#### 視点移動
* 視点移動：マウスホイールをクリックしながら移動\
* 視点回転：マウスホイールをクリックしながらShift+移動\
* モデルが表示さている画面の右上にあるサイコロを押すと回転(メタセコで言うF1,F2,F3ができる)\
* 家のマークを押すとデフォルト位置に移動
#### 軸の表示
画面左タブのOriginファイルに軸と面がある\
選択し表示設定[Alt+V]で表示/非表示を切り替える\
全体の軸と個別の軸がある\
[参考スクショ](images/inventor/軸の表示.PNG)

### 流れ
1. Inventorをインストールをする
2. ラズパイマウスのCADデータのダウンロードする
3. InventorでラズパイマウスのCADデータを(.iam)ファイルで保存
4. ホイールとボディのアセンブリファイル()を作成
5. ホイールの位置合わせをする   [参考スクショ](images/inventor/タイヤの位置合わせ.PNG)
6. ボディの位置合わせをする
   [参考スクショ1](images/inventor/ボディの位置合わせ.PNG)   [参考スクショ2](images/inventor/ボディの位置合わせ2.PNG)
7. blenderでメッシュを結合する
8. イナーシャ・重さの値を確認する
9. urdfで表示

### 1.Inventorをインストールをする
今回用いる[Inventor Professional](https://www.autodesk.co.jp/education/free-software/inventor-professional)はオートデスクが販売するCADソフト。\
サインインしてWindows64bit版をダウンロードする

### 2.ラズパイマウスのCADデータのダウンロードする
ここの[Github](https://github.com/rt-net/RaspberryPiMouse_Hardware)から\
/cat_data/RasPiMouose.igsファイルをInventorで開く(インポートする)\
OKをひたすら押す

### 3. InventorでラズパイマウスのCADデータを(.iam)ファイルで保存
[ファイル]->[名前をつけて保存]で適当なディレクトリを作成しアセンブリファイルで保存する

### 4. ホイールとボディのアセンブリファイル()を作成
1. ホイール用のアセンブリファイルを新規作成する   [参考スクショ](images/inventor/アセンブリファイルの作成.PNG)\
[ファイル]->[新規作成]->[アセンブリ]->[Standard.iam]->[作成]
2. ホイール用のアセンブリファイルにタイヤ(ゴムで出来た黒い部分)とホイール(軸の銀色の部分)をコピペ\
画面下リボンから先ほど開いていたRasPiMouse.iamを選択し表示   [参考スクショ](images/inventor/アセンブリファイルの移動.PNG)\
Wheel5_ネジ穴位置改良:1 タイヤφ44用7.5mm幅_RT-T75-44-R:1の2つのパーツを選択\
ホイール用のアセンブリファイルにコピー＆ペースト(Cntl+C, Cntl+P)\
名前をつけて保存
3. 1,2と同様にボディ用のアセンブリファイルを作成し、ステッピングモータ、アルミフレーム以下略をコピペ\
名前をつけて保存

### 5. ホイールの位置合わせをする
* タイヤ・ホイールの中心を揃える。この時タイヤがY軸方向に回るようにする
    * 位置合わせには画面植えリボンの[アセンブリ]->[拘束]を選択する
    * 使用するのは基本的に[タイプ]->[メイト]と[タイプ]->[角度]のみ
    * [メイト]は[解析]->[メイト]、[角度]は[解析]->[有方向]、ダメならもうひとつの方
    * 選択したい面・線にカーソルを合わせて少し待つと選択肢が表示される

* タイヤの位置合わせ
    1. メイト拘束で、タイヤ外側の面の[面の法線]と全体の[XZ平面]を選択
    2. 角度拘束で、タイヤ中心の[Z軸]と全体の[Z軸]を選択

* ホイールの位置合わせ
    1. メイト拘束で、ホイール外側の面の[面の法線]と全体の[XZ平面]を選択
    2. 角度拘束で、タイヤ中心の[Z軸]と全体の[Z軸]を選択
    3. 角度拘束で、タイヤの[XZ平面]と全体の[XZ平面]を選択

すべて終わったら[ファイル]->[書き出し]->[CAD形式]->[ファイルの種類：OBJファイル]で保存する\
mtlファイルも自動的に作成される。

### 6. ボディの位置合わせをする
ボディの底面が地面になるように揃える。この時、モータ軸の中心がXが0に来るようにする

1. メイト拘束で、アルミフレームの底面の[面の法線]と全体の[XY平面]を選択
2. 角度拘束で、ステッピングモータの[Y軸]と全体の[Y軸]を選択
3. メイト(フラッシュ)拘束でアルミフレームの[XZ平面]と全体の[XZ平面]を選択
4. メイト(フラッシュ)拘束でアルミフレームの[YZ平面]と全体の[YZ平面]を選択

すべて終わったら[ファイル]->[書き出し]->[CAD形式]->[ファイルの種類：OBJファイル]で保存する\
mtlファイルも自動的に作成される。

### 7. blenderでメッシュを結合する
部品が多いオブジェクトだとマテリアルが表示されないことがあるのでblederで部品を合体させる、謎
1. blenderを起動、最初の箱も消す[deleteキー]->[vertical]
2. objファイルを開く、[A]で全選択して左リボンの[Tools]->[Edit]->[join]
3. 上書き保存

### 8. イナーシャ・重さの値を確認する
Inventorからイナーシャ・重さの値を計算する。\
左リボンの[AssenmlyXX.iam]->[iProperty]->[物理情報]->[更新]を選択\
この時、表示された単位がKg/meter/rad担っていることを確認する、なっていなければ[Inventorの表示単位設定](#Inventorの表示単位設定)
をする。\
クリップボードにコピーしてメモ帳などで保存する\
urdfで表示するために必要な値は以下
```
一般的なプロパティ：質量[kg] -> mass value
重心に関する完成の質量モーメント-> Ixx, Ixy, Ixz, Iyy, Iyz, Izz -> inertia
```

### 9. urdfで表示
チュートリアルのラズパイマウスを作成したラズパイマウスのデータに置き換える\
/raspimouse_sim/raspimouse_description/meshesの下に以下のようにファイルを配置する
```
/meshes
    -/obj
        -/wheel
            -raspi_wheel.obj
            -raspi_wheel.mtl
        -/body
            -raspi_body.obj
            -raspi_body.mtl
```
書き換えるファイルはホイールのxacro、ボディのxacro、全体のxacroファイル
* wheel.urdf.xacro
```
  <link name="${prefix}_wheel">
    <visual>
      <geometry>
+        <!-- <mesh filename="package://raspimouse_description/meshes/dae/wheel/RasPiMouse_wheel.dae" /> -->
+        <mesh filename="package://raspimouse_description/meshes/obj/wheel/raspi_wheel.obj" />
      </geometry>
    </visual>
    <collision>
+        <origin xyz="0 0 0" rpy="${pi / 2} 0 0" />
        <geometry>
          <cylinder radius="${wheel_radius}" length="${wheel_length}" />
          <!-- <mesh filename="package://raspimouse_description/meshes/dae/wheel/RasPiMouse_wheel.dae" /> -->
        </geometry>
    </collision>
      <inertial>
+        <mass value="3.16354494"/>
        <origin xyz="-0.000107 0 0.002875" />
        <inertia
+              ixx="0.02296002"
+              ixy="0.00000000" iyy="0.04452360"
+              ixz="0.00000000" iyz="-0.00000001" izz="0.02296002"/>

```

* body.urdf.xacro
```
    <link name="base_link">
      <visual>
        <geometry>
+          <!-- <mesh filename="package://raspimouse_description/meshes/dae/body/RasPiMouse_body.dae" /> -->
+          <mesh filename="package://raspimouse_description/meshes/obj/body/raspi_body.obj" />
        </geometry>
      </visual>
      <collision>
        <geometry>
+          <!-- <mesh filename="package://raspimouse_description/meshes/dae/body/RasPiMouse_body.dae" /> -->
+          <mesh filename="package://raspimouse_description/meshes/obj/body/raspi_body.obj" />
        </geometry>
      </collision>
      <inertial>

+        <mass value="31.20594944"/>
        <inertia
+              ixx="1.79583172"
+              ixy="0.03278618" iyy="1.55345103"
+              ixz="0.02535482" iyz="0.01272625" izz="1.22908514"/>
```

* raspimouse.urdf.xacro
```
  <!-- Wheel -->
  <xacro:wheel prefix="right" parent="base_link">
+    <!-- <origin xyz="0 -0.0425 0.02215" rpy="1.57 0 0" /> -->
+    <origin xyz="0 -0.0425 0.02215" rpy="0 0 0" />
+    <axis xyz="0 -1 0" />
  </xacro:wheel>
  <xacro:wheel prefix="left" parent="base_link">
+    <!-- <origin xyz="0 0.0425 0.02215" rpy="-1.57 0 0" /> -->
+    <origin xyz="0 0.0425 0.02215" rpy="0 0 0" />
+    <axis xyz="0 1 0" />
  </xacro:wheel>
```


### 所感　時間がかかったこと、難しかった、やりたいことなど
<!-- 
[解決/未解決/所感/疑問/参考など]
[参考][記事名](URL)
 -->
[所感] Inventorの使い方に慣れる
[所感] 固定するときの軸が難しい


---
---
##### 以下テンプレート
## ROS0X_
### 概要
<!-- 何をするもの、方法と結果を３行程度 -->

### 所感　時間がかかったこと、難しかった、やりたいことなど
<!-- 
[解決/未解決/所感/疑問/参考など]
[参考][記事名](URL)
 -->
### メモ

[トップへ](#本日やったこと)
