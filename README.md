# Revolute

Arduino Mega 2560を使用し、Gimmickの回転角度に応じて、指定した距離だけ機構を降下させるプログラムです。

## 動作概要

Gimmickの角度範囲が変わったときに、モータへ降下指令を出します。

| Gimmickの角度 | 範囲へ移ったときの動作 |
|---|---|
| 120°以下 | 新しい移動指令なし |
| 120°超〜240°以下 | 100mm降下 |
| 240°超〜360°以下 | 50mm降下 |
| 360°超 | 50mm降下 |

- 角度は起動位置を基準とした累積値です。
- 360°を超えても0°には戻りません。
- 逆回転で範囲を戻った場合も、移動先の範囲に応じた指令が発生します。
- 動作中に受け取った新しい降下指令は無視され、保存されません。
- `DriveMotor(0)`は停止命令ではなく、何もしません。

## ファイル構成

```text
Revolute/
├── Revolute.ino
├── PinAssign.h
├── PinAssign.cpp
├── Encoder.h
├── Encoder.cpp
├── DriveMotor.h
├── DriveMotor.cpp
└── README.md
```

| ファイル | 役割 |
|---|---|
| `Revolute.ino` | 角度範囲の判定、降下指令、制御更新 |
| `PinAssign.h` / `.cpp` | ピン定義、入出力設定、割り込み登録 |
| `Encoder.h` / `.cpp` | エンコーダのカウント、角度取得 |
| `DriveMotor.h` / `.cpp` | 目標角度の設定、P制御、PWM出力 |

## ピン配置

以下は、GimmickのエンコーダにD18・D19を使用する構成です。実際の配線と`PinAssign.h`を一致させてください。

| 用途 | 定数名 | Megaのピン |
|---|---|---|
| モータPWM | `PWMA` | D13 |
| モータ方向 | `AIN2` | D12 |
| モータ方向 | `AIN1` | D11 |
| モータ待機 | `STBY` | D10 |
| モータエンコーダA相 | `Motor_A` | D2 |
| モータエンコーダB相 | `Motor_B` | D3 |
| GimmickエンコーダA相 | `Gimmick_A` | D18 |
| GimmickエンコーダB相 | `Gimmick_B` | D19 |

> MegaのD4・D5は`attachInterrupt()`に対応していません。  
> D18・D19をエンコーダに使用する場合、同じピンでSerial1は使用できません。

## エンコーダの読み取り

A・B相の立ち上がりと立ち下がりを外部割り込みで検出します。

両相の状態から回転方向を判定し、1台につき1つのカウント値を増減します。A相1周期につき4回カウントする「4逓倍」方式です。

```cpp
float motorAngle = GetMotorAngle();
float gimmickAngle = GetGimmickAngle();
```

角度の計算式は次のとおりです。

```text
角度[°] = カウント値 × 360 ÷ 1回転あたりのカウント数
```

### モータ側の換算値

使用モータ：CHR-GM25-370（DC12V・350rpm）

現在のコードでは、次の仕様を前提にしています。

- エンコーダ：11パルス／モータ軸1回転
- 読み取り方式：4逓倍
- 公称減速比：34

```text
出力軸1回転のカウント数 = 11 × 4 × 34 = 1496
```

```cpp
float angle = count * 360.0f / 1496.0f;
```

1496は公称仕様に基づく値です。購入品の仕様や実測結果に合わせて確認してください。

Gimmick側には、そのエンコーダの分解能・減速比・取り付け位置に合った換算値を設定します。

## 降下量の指定とモータ制御

`DriveMotor(length)`の引数は、現在位置からの降下量[mm]です。

```cpp
DriveMotor(100.0f);  // 現在位置から100mm降下
```

指定距離を回転量へ変換し、動作開始時の角度から目標角度を設定します。

```text
回転量[°] = 降下量[mm] × 360 ÷ TravelPerRev
目標角度 = 開始角度 + DownSign × 回転量
```

`UpdateMotor()`で角度偏差に比例するPWMを出力します。現在はPID制御のうち、P項のみを使用しています。

```text
角度偏差 = 目標角度 − 現在角度
PWM指令 = Kp × 角度偏差
```

PWM指令を`±MaxPWM`に制限し、角度偏差が`±AngleTolerance`以内になると停止指令を出して制御を終了します。

```cpp
void loop() {
    // Gimmickの角度判定・動作開始指令

    UpdateMotor();  // 毎回呼び出す
}
```

### 調整項目

| 定数 | 意味 |
|---|---|
| `TravelPerRev` | 出力軸1回転あたりの降下量[mm] |
| `DownSign` | 降下時に角度が増えるなら`+1`、減るなら`-1` |
| `Kp` | 角度偏差に対する比例ゲイン |
| `AngleTolerance` | 停止と判定する角度誤差[°] |
| `MaxPWM` | 制御に使用するPWMの上限 |

`TravelPerRev = 100.0f`は仮値です。実際の機構に合わせて変更してください。

正のPWMでエンコーダ角度が増えるよう、駆動方向とエンコーダの符号を合わせます。

### 現在の制御の制限

- 摩擦により、目標角度に達する前に止まる場合があります。
- 停止判定後に荷重で位置がずれても、自動では補正しません。
- ワイヤの重ね巻きなどで巻き取り径が変わる場合、一定の`TravelPerRev`では距離に誤差が生じます。
- 停止指令で荷重を保持できるかは、ドライバと機構に依存します。

## Visual Studio Codeでのコンパイル

### 1. Arduino CLIの確認

VS Codeの「ターミナル」→「新しいターミナル」を開きます。

```cmd
arduino-cli version
```

コマンドが認識されない場合は、Arduino CLIのインストールとPATH設定を確認してください。

### 2. ボードパッケージの導入

Arduino AVR Boardsが未導入の場合に実行します。

```cmd
arduino-cli core update-index
arduino-cli core install arduino:avr
```

### 3. コンパイル

VS Codeで、`Revolute.ino`が入っている`Revolute`フォルダーを開きます。

ターミナルの現在位置がそのフォルダーになっている状態で、次を実行します。

```cmd
arduino-cli compile --fqbn arduino:avr:mega "."
```

親フォルダーから実行する場合は、次のように指定します。

```cmd
arduino-cli compile --fqbn arduino:avr:mega ".\Revolute"
```

> コンパイルのみでは、ボードへの書き込みは行われません。

### 4. ボードへの書き込み

接続したボードのポートを確認します。

```cmd
arduino-cli board list
```

表示されたポートを指定して書き込みます。`COM3`は例なので、実際のポートへ変更してください。

```cmd
arduino-cli upload -p COM3 --fqbn arduino:avr:mega "."
```

## よくあるエラー

| エラー | 確認事項 |
|---|---|
| `arduino-cli`が認識されない | Arduino CLIのインストールとPATH設定 |
| `Can't open sketch` | 現在位置とスケッチフォルダーの指定 |
| `PinAssign.h: No such file or directory` | ファイルの配置と`#include`の形式 |
| 関数が宣言されていない | 対応するヘッダーの読み込みと関数宣言 |

プロジェクト内のヘッダーは、ダブルクォーテーションで読み込みます。

```cpp
#include "PinAssign.h"
#include "Encoder.h"
#include "DriveMotor.h"
```

スケッチフォルダー名とメインファイル名は、`Revolute`と`Revolute.ino`のように一致させてください。
