/*
 Fading

 This example shows how to fade an LED using the analogWrite() function.

 The circuit:
 * LED attached from digital pin 9 to ground.

 Created 1 Nov 2008
 By David A. Mellis
 modified 30 Aug 2011
 By Tom Igoe

 http://www.arduino.cc/en/Tutorial/Fading

 This example code is in the public domain.

 */
#include "ArduinoNunchuk.h"
// 赤: , 緑: , 白: , 黄: 


// ピン設定
int trgPin   = 4;    // D:トリガー
int cvPin    = 5;    // A:キーボード音程(980Hz)
int pitchPin = 6;    // A:ピッチベンド, ビブラート
int hpfPin   = 9;    // A:ハイパスフィルタ
int lpfPin   = 10;   // A:ローパスフィルタ

// ヌンチャク設定
#define BAUDRATE 19200
ArduinoNunchuk nunchuk = ArduinoNunchuk();

// 音程テーブル
int freq_tbl[] = {
  51, 54, 57, 61,
  64, 68, 72, 76,
  81, 86, 91, 96,
  102, 108, 114, 121,
  129, 136, 144, 153,
  162, 172, 182, 193,
  204, 216, 229, 243,
};

// テスト用フレーズ(ドシドドシドソミ レシソﾌｧソシドシ)
int doremi_tbl[] = {
  0,  10, 12,  0, 10, 12,  7, 15,
  14, 10,  7,  5,  7, 10, 12, 10,
};
// テスト用フレーズ用カウンタ
const int count_max = 120;
int count = 0;

void setup() {
  // ヌンチャク設定
  Serial.begin(BAUDRATE);
  nunchuk.init();
}

void loop() {

  // フレーズ長さ
  int len = sizeof(doremi_tbl) / sizeof(doremi_tbl[0]);

  // fade in from min to max in increments of 5 points:
  for (int i = 0; i < len; i++) {
    nunchuk.update();
    int freq = freq_tbl[ doremi_tbl[i] ];
    // sets the value (range from 0 to 255):
    analogWrite(cvPin, freq);   // 5
    analogWrite(pitchPin, nunchuk.analogY);  // 6
    // wait for 30 milliseconds to see the dimming effect
    delay(120);
  }

  Serial.print(nunchuk.analogY, DEC);
  Serial.print(' ');
  // fade out from max to min in increments of 5 points:
  //for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    // analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    // delay(30);
  //}
}


