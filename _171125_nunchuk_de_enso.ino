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


int ledPin = 9;    // LED connected to digital pin 9

int freq_tbl[] = {
  51, 54, 57, 61,
  64, 68, 72, 76,
  81, 86, 91, 96,
  102, 108, 114, 121,
  129, 136, 144, 153,
  162, 172, 182, 193,
  204, 216, 229, 243,
};

int doremi_tbl[] = {
  0, 10, 12, 0, 10, 12, 7, 10,
};


void setup() {
  // nothing happens in setup
}

void loop() {
  int max = sizeof(doremi_tbl) / sizeof(doremi_tbl[0]);
  // fade in from min to max in increments of 5 points:
  for (int i = 0; i < max; i++) {
    int freq = freq_tbl[ doremi_tbl[i] ];
    // sets the value (range from 0 to 255):
    analogWrite(ledPin, freq);
    // wait for 30 milliseconds to see the dimming effect
    delay(120);
  }

  // fade out from max to min in increments of 5 points:
  //for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) {
    // sets the value (range from 0 to 255):
    // analogWrite(ledPin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    // delay(30);
  //}
}


