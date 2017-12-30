#include "ArduinoNunchuk.h"
// 緑: SDA(A4), 黄: SCL(A5)

// ピン設定
int trigPin  = 4;    // D:トリガー
int cvPin    = 5;    // A:キーボード音程(980Hz)
int pitchPin = 6;    // A:ピッチベンド, ビブラート  : スティック, 振り
int hpfPin   = 9;    // A:ハイパスフィルタ          : c + 右に90度
int lpfPin   = 10;   // A:ローパスフィルタ          : c + 左に90度
// ※analogWrite 最大値 : 255
// PWM出力の周波数
// 5, 6     : 980Hz
// 4, 9, 10 : 490Hz
// CRフィルタの値
// C:1uF
// R:47K

// ヌンチャク設定
ArduinoNunchuk nunchuk = ArduinoNunchuk();
/*  取れる値は "6" 種類。
    int analogX;  // スティック (中心:128, 最低:約34...物理的に)
    int analogY;
    int accelX;   // 加速度 (中心:512)
    int accelY;
    int accelZ;
    int zButton;  // ボタン
    int cButton;
*/
// 入力用定数
const int ANAOFS = 127;
const int DEADZONE = 12;
const int ACCOFS = 511;
// ヌンチャク用 変数
float accelRoll  = 0.0;
float accelPitch = 0.0;
int accelVelo  = 0;


// 音程テーブル (28音)
int freq_tbl[] = {
  51,  54,  57,  61,  // 1オクターブ目
  64,  68,  72,  76,
  81,  86,  91,  96,
  102, 108, 114, 121, // 2オクターブ目
  129, 136, 144, 153,
  162, 172, 182, 193,
  204, 216, 229, 243, // 3オクターブ目
};

// テスト用フレーズ
const int doremi_tbl[] = {
  0,  10, 12,  0, 10, 12,  7, 15, // ドシドドシドソミ
  14, 10,  7,  5,  7, 10, 12, 10, // レシソﾌｧソシドシ
};
const int doremi_len = sizeof(doremi_tbl) / sizeof(doremi_tbl[0]);

// テスト用フレーズ 変数
int count = 0;              // カウンタ
const int count_max = 4;    // 音長(短いと早く)
int step = 0;               // 今、何番目のフレーズ音か


void setup() {
  // デジタルピン設定
  pinMode(trigPin, OUTPUT);
  // ヌンチャク設定
  nunchuk.init();

  // Debug用ポート
  Serial.begin(9600);
}

void loop() {
  // ヌンチャク取得
  nunchuk.update();
  // 拡張値 計算
  accelRoll  = atan2(nunchuk.accelX - ACCOFS, nunchuk.accelZ - ACCOFS) / PI;
  accelPitch = atan2(nunchuk.accelY - ACCOFS, nunchuk.accelZ - ACCOFS) / PI;
  accelVelo  = sqrt(  pow(nunchuk.accelX - ACCOFS, 2) 
                    + pow(nunchuk.accelY - ACCOFS, 2) 
                    + pow(nunchuk.accelZ - ACCOFS, 2) );

  // 計算
  // trig, cv
  int trig = nunchuk.zButton;
  int cv_freq = freq_tbl[ doremi_tbl[step] ];

  // pitch
  int isDead = abs(nunchuk.analogX - ANAOFS) < DEADZONE;
  float ana_value = 0.0;
  if (!isDead) {
    int sgn = (nunchuk.analogX > ANAOFS) - (nunchuk.analogX < ANAOFS); // 倒した向き
    // 2/3程度倒せばMAXとなるように1.75倍する
    ana_value = (nunchuk.analogX - ANAOFS - (DEADZONE * sgn)) * 1.75;
    ana_value = min(ana_value,  127.0);
    ana_value = max(ana_value, -127.0);
  }
  int pitch_freq = (int)min((int)ana_value + ANAOFS, 255);
  // int pitch_freq = accelVelo;
  // int vibrate = (int)(ACCOFS + ACCOFS * nunchuk.accelX) >> 2;

  // lpf, hpf
  // r_roll : 右回転
  float r_roll = 0.0;
  if (accelRoll > 0.0) // 右に傾いている場合のみ有効
    r_roll = accelRoll * 2.0; // 半回転でMAXとなるように2で掛けます。
  r_roll = min(r_roll, 1.0);
  // l_roll : 左回転
  float l_roll = 0.0;
  if (accelRoll < 0.0) // 左に傾いている場合のみ有効
    l_roll = -accelRoll * 2.0;
  l_roll = min(l_roll, 1.0);
  int hpf_freq = min((int)(256 * r_roll), 255);
  int lpf_freq = min((int)(256 - (256 * l_roll)), 255);

  // 書込!
  digitalWrite(trigPin,   trig);
  analogWrite (cvPin,     cv_freq);
  analogWrite (pitchPin,  pitch_freq);
  analogWrite (hpfPin,    hpf_freq);
  analogWrite (lpfPin,    lpf_freq);

  // 音長の処理
  count++;
  if (count >= count_max) {
    step++;
    count = 0;
  }

  // フレーズ:最後まで到達したら戻す
  if (step >= doremi_len) {
    step = 0;
  }

  // 確認
  Serial.print("pitch_freq:");
  Serial.println(pitch_freq);
  Serial.print("lpf_freq:");
  Serial.println(lpf_freq);
}


