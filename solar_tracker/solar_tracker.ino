#include <Servo.h>
#include <SoftwareWire.h>
#include <LiquidCrystal_SoftI2C.h>

// =====================
// SERVOS
// =====================
Servo servoH;
Servo servoV;

// =====================
// LDR PINS
// =====================
const int A0_pin = A0; // Top Right
const int A1_pin = A1; // Bottom Left
const int A2_pin = A2; // Bottom Right
const int A3_pin = A3; // Top Left

// =====================
// VOLTAGE SENSOR
// =====================
const int voltagePin = A4;

// =====================
// MODE SWITCH
// D4 -> switch -> GND
// =====================
const int modeSwitch = 4;

// =====================
// POSITIONS
// =====================
int hPos = 90;
const int hMin = 4;
const int hMax = 170;

int vPos = 50;
const int vUp = 60;
const int vDown = 10;

// =====================
// TUNING
// =====================
const int tolerance = 60;
const int stepSize = 3;

// =====================
// LCD
// =====================
#define CUSTOM_SDA 11
#define CUSTOM_SCL 12

SoftwareWire myWire(CUSTOM_SDA, CUSTOM_SCL);
LiquidCrystal_I2C lcd(0x27, 16, 2, &myWire);

// ======================================================
// SMOOTH ANALOG READ
// ======================================================
int smoothRead(int pin) {

  long total = 0;

  for (int i = 0; i < 10; i++) {
    total += analogRead(pin);
    delay(2);
  }

  return total / 10;
}

void setup() {

  Serial.begin(9600);

  // Switch input
  pinMode(modeSwitch, INPUT_PULLUP);

  // LCD
  lcd.begin();
  lcd.backlight();

  // SAFE START POSITION
  servoV.write(vPos);
  servoH.write(hPos);

  // ATTACH SERVOS
  servoH.attach(9);
  servoV.attach(10, 500, 1500);

  delay(500);

  lcd.clear();
}

void loop() {

  // =====================
  // READ SWITCH
  // =====================
  bool dualAxis = (digitalRead(modeSwitch) == LOW);

  // =====================
  // READ LDRs (SMOOTHED)
  // =====================
  int tr = smoothRead(A0_pin);
  int bl = smoothRead(A1_pin);
  int br = smoothRead(A2_pin);
  int tl = smoothRead(A3_pin);

  // =====================
  // DEBUG
  // =====================
  Serial.print("TL: ");
  Serial.print(tl);

  Serial.print(" | TR: ");
  Serial.print(tr);

  Serial.print(" | BL: ");
  Serial.print(bl);

  Serial.print(" | BR: ");
  Serial.println(br);

  // =====================
  // AVERAGES
  // =====================
  int topAvg = (tl + tr) / 2;
  int botAvg = (bl + br) / 2;

  int leftAvg = (tl + bl) / 2;
  int rightAvg = (tr + br) / 2;

  // =====================
  // ERRORS
  // =====================
  int vertError = botAvg - topAvg;
  int horizError = leftAvg - rightAvg;

  // =====================
  // HORIZONTAL TRACKING
  // =====================
  if (abs(horizError) > tolerance) {

    if (leftAvg > rightAvg) {
      hPos -= stepSize;
    }

    else {
      hPos += stepSize;
    }
  }

  // LIMITS
  hPos = constrain(hPos, hMin, hMax);

  // WRITE ONLY IF CHANGED
  static int lastH = -1;

  if (hPos != lastH) {
    servoH.write(hPos);
    lastH = hPos;
  }

  // =====================
  // VERTICAL TRACKING
  // ONLY IN DUAL AXIS MODE
  // =====================
  if (dualAxis) {

    if (abs(vertError) > tolerance) {

      if (botAvg > topAvg) {
        vPos -= stepSize;
      }

      else {
        vPos += stepSize;
      }
    }

    // LIMITS
    vPos = constrain(vPos, vDown, vUp);

    // WRITE ONLY IF CHANGED
    static int lastV = -1;

    if (vPos != lastV) {
      servoV.write(vPos);
      lastV = vPos;
    }
  }

  // =====================
  // VOLTAGE SENSOR
  // =====================
  int sensorValue = analogRead(voltagePin);

  float voltage =
    sensorValue * (5.0 / 1023.0) * 5.0;

  // =====================
  // LCD
  // =====================
  lcd.setCursor(0, 0);
  lcd.print("Volt:");
  lcd.print(voltage, 2);
  lcd.print("V   ");

  lcd.setCursor(0, 1);

  if (dualAxis) {
    lcd.print("Dual Axis     ");
  }

  else {
    lcd.print("Single Axis   ");
  }

  // =====================
  // SPEED CONTROL
  // =====================
  delay(5);
}