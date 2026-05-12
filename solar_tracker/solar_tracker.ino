#include <Servo.h>
#include <SoftwareWire.h>
#include <LiquidCrystal_SoftI2C.h>

// =====================
// SERVOS
// =====================
Servo servoH; // D9 (Horizontal)
Servo servoV; // D10 (Vertical)

// =====================
// LDR PINS
// =====================
const int A0_pin = A0; // Top Right
const int A1_pin = A1; // Bottom Left
const int A2_pin = A2; // Bottom Right
const int A3_pin = A3; // Top Left
const int voltagePin = A4;

// =====================
// POSITIONS
// =====================
int hPos = 90;
const int hMin = 10;
const int hMax = 170;

int vPos = 80;
const int vUp = 100;
const int vDown = 60;

// =====================
// TUNING
// =====================
const int tolerance = 30;
const int stepSize = 3;

// =====================
// LCD TIMING
// =====================
unsigned long lastLCDUpdate = 0;
unsigned long lastLCDInit = 0;
const unsigned long LCD_INTERVAL = 200;
const unsigned long LCD_REINIT_INTERVAL = 5000;

// =====================
// LCD (Software I2C on D2/D3)
// =====================
#define CUSTOM_SDA 2
#define CUSTOM_SCL 3
SoftwareWire myWire(CUSTOM_SDA, CUSTOM_SCL);
LiquidCrystal_I2C lcd(0x27, 16, 2, &myWire);

void setup() {
  servoH.attach(9);
  servoV.attach(10);

  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();

  servoH.write(hPos);
  servoV.write(vPos);

  delay(1500);
  lcd.clear();
}

void loop() {

  // =====================
  // READ LDRs (averaged x3 to reduce jitter)
  // =====================
  int tr = (analogRead(A0_pin) + analogRead(A0_pin) + analogRead(A0_pin)) / 3;
  int bl = (analogRead(A1_pin) + analogRead(A1_pin) + analogRead(A1_pin)) / 3;
  int br = (analogRead(A2_pin) + analogRead(A2_pin) + analogRead(A2_pin)) / 3;
  int tl = (analogRead(A3_pin) + analogRead(A3_pin) + analogRead(A3_pin)) / 3;

  // =====================
  // SERIAL DEBUG
  // =====================
  Serial.print("TL: "); Serial.print(tl);
  Serial.print(" | TR: "); Serial.print(tr);
  Serial.print(" | BL: "); Serial.print(bl);
  Serial.print(" | BR: "); Serial.println(br);

  // =====================
  // CALCULATIONS
  // =====================
  int topAvg   = (tl + tr) / 2;
  int botAvg   = (bl + br) / 2;
  int leftAvg  = (tl + bl) / 2;
  int rightAvg = (tr + br) / 2;

  int vertError  = botAvg - topAvg;
  int horizError = leftAvg - rightAvg;

  // =====================
  // HORIZONTAL SERVO (D9)
  // =====================
  if (abs(horizError) > tolerance) {
    if (leftAvg > rightAvg && hPos > hMin) {
      hPos -= stepSize;
    } else if (rightAvg > leftAvg && hPos < hMax) {
      hPos += stepSize;
    }
  }
  hPos = constrain(hPos, hMin, hMax);
  servoH.write(hPos);

  // =====================
  // VERTICAL SERVO (D10)
  // =====================
  if (abs(vertError) > tolerance) {
    if (botAvg > topAvg && vPos > vDown) {
      vPos -= stepSize;
    } else if (topAvg > botAvg && vPos < vUp) {
      vPos += stepSize;
    }
  }
  vPos = constrain(vPos, vDown, vUp);
  servoV.write(vPos);

  // =====================
  // LCD DISPLAY (non-blocking + auto-reinit)
  // =====================
  unsigned long now = millis();
  if (now - lastLCDUpdate >= LCD_INTERVAL) {
    lastLCDUpdate = now;

    // reinit every 5s to recover from noise glitches
    if (now - lastLCDInit >= LCD_REINIT_INTERVAL) {
      lastLCDInit = now;
      lcd.begin();
      lcd.backlight();
    }

    // voltage read only when needed
    analogRead(voltagePin);
    delay(5);
    int sensorValue = analogRead(voltagePin);
    float voltage = sensorValue * (5.0 / 1023.0) * 5.0;

    lcd.setCursor(0, 0);
    lcd.print("Volt:");
    lcd.print(voltage, 2);
    lcd.print("V   ");

    lcd.setCursor(0, 1);
    lcd.print("H:");
    lcd.print(hPos);
    lcd.print(" V:");
    lcd.print(vPos);
    lcd.print("     ");
  }
}