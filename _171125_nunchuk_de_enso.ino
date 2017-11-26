#include "ArduinoNunchuk.h"
// 緑: SDA(A4), 黄: SCL(A5)

// ピン設定
int trigPin  = 4;    // D:トリガー
int cvPin    = 5;    // A:キーボード音程(980Hz)
int pitchPin = 6;    // A:ピッチベンド, ビブラート
int hpfPin   = 9;    // A:ハイパスフィルタ
int lpfPin   = 10;   // A:ローパスフィルタ

// ヌンチャク設定
ArduinoNunchuk nunchuk = ArduinoNunchuk();

// 音程テーブル (28音)
int freq_tbl[] = {
  51, 54, 57, 61,     // 1オクターブ目
  64, 68, 72, 76,
  81, 86, 91, 96,
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

// テスト用フレーズ用カウンタ
int count = 0;
const int count_max = 12;
int step = 0;   // 今、何番目のフレーズ音か


void setup() {
  // デジタルピン設定
  pinMode(trigPin, OUTPUT);
  // ヌンチャク設定
  nunchuk.init();
}

void loop() {
  // ヌンチャク取得
  nunchuk.update();

  // 計算
  int trig = nunchuk.zButton;
  int cv_freq = freq_tbl[ doremi_tbl[step] ];
  int pitch_freq = nunchuk.analogY;
  int hpf_freq = 0; // TODO
  int lpf_freq = 0; // TODO

  // 書込!
  digitalWrite(trigPin,  trig);
  analogWrite (cvPin,    cv_freq);
  analogWrite (pitchPin, pitch_freq);
  analogWrite (hpfPin, hpf_freq);
  analogWrite (lpfPin, lpf_freq);

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
}


