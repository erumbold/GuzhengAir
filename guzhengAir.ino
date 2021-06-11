#include <NewPing.h>
#include <Keypad.h>

// main ultrasonic sensor
#define triggerPinMain A5
#define echoPinMain A4
#define maxDistanceMain 25

// side ultrasonic sensor
#define triggerPinSide A2
#define echoPinSide A3
#define maxDistanceSide 20

NewPing sonarMain(triggerPinMain, echoPinMain, maxDistanceMain);
NewPing sonarSide(triggerPinSide, echoPinSide, maxDistanceSide);

// keypad vars
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {3, 4, 5, 6};
byte colPins[COLS] = {7, 8, 12, 13};
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// 2-color LED vars
const int redPin2 = A1;
const int yellowPin2 = A0;

// RGB LED vars
const int RED_PIN = 11;
const int GREEN_PIN = 10;
const int BLUE_PIN = 9;

bool isTrillOn = false;
bool isGlissOn = false;
int keyCode = 0;

void setup() {
  // main ultrasonic
  pinMode(triggerPinMain, OUTPUT);
  pinMode(echoPinMain, INPUT);

  // 2-color LED
  pinMode(redPin2, OUTPUT);
  pinMode(yellowPin2, OUTPUT);
  

  // RGB LED
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  delay(10);
  // ULTRASONIC
  unsigned int uSMain = sonarMain.ping();
  unsigned int uSSide = sonarSide.ping();

  int b1 = uSMain >> 7;
  int b2 = uSMain & 127;
  int b3 = uSSide >> 3;
  int b4 = uSSide & 7;

  Serial.write(b1);
  Serial.write(b2);
  Serial.write(b3);
  Serial.write(b4);
  
  // KEYPAD
  char customKey = customKeypad.getKey();

  if (customKey == 'A') {
    isTrillOn = !isTrillOn;
    isGlissOn = false;
    analogWrite(redPin2, 0);
    analogWrite(yellowPin2, 255);
  } else if (customKey == 'B') {
    isGlissOn = !isGlissOn;
    isTrillOn = false;
    analogWrite(redPin2, 255);
    analogWrite(yellowPin2, 0);
  } else if (customKey == 'C') {
    isTrillOn = false;
    isGlissOn = false;
    analogWrite(redPin2, 0);
    analogWrite(yellowPin2, 0);
  }
  
  int newKeyCode = int(customKey);
  if (newKeyCode != 0) {
    Serial.write(newKeyCode);
    keyCode = newKeyCode;
  } else {
    Serial.write(keyCode);
  }

  Serial.write(255);
}
