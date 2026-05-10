#include <Servo.h>
#include <SoftwareWire.h>
#include <LiquidCrystal_SoftI2C.h>

// =====================
// SERVOS
// =====================
Servo servoH; // D9 (Horizontal)
Servo servoV; // D10 (Vertical)

// =====================
// LDR PINS (YOUR MAPPING)
// =====================
int A0_pin = A0; // Top Right
int A1_pin = A1; // Bottom Left
int A2_pin = A2; // Bottom Right
int A3_pin = A3; // Top Left

int voltagePin = A4;

// =====================
// POSITIONS
// =====================
int hPos = 90;
int hMin = 40;
int hMax = 140;

int vPos = 80;
int vUp = 100;
int vDown = 60;

// =====================
// TUNING
// =====================
int tolerance = 30;
int stepSize = 2;

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
  // READ LDRs
  // =====================
  int tr = analogRead(A0_pin); // Top Right
  int bl = analogRead(A1_pin); // Bottom Left
  int br = analogRead(A2_pin); // Bottom Right
  int tl = analogRead(A3_pin); // Top Left

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
  int topAvg = (tl + tr) / 2;
  int botAvg = (bl + br) / 2;
  int leftAvg = (tl + bl) / 2;
  int rightAvg = (tr + br) / 2;

  int vertError = botAvg - topAvg;
  int horizError = leftAvg - rightAvg;

  // =====================
  // HORIZONTAL SERVO (D9)
  // =====================
  if (abs(horizError) > tolerance) {
    if (leftAvg > rightAvg && hPos > hMin) {
      hPos -= stepSize;
    }
    else if (rightAvg > leftAvg && hPos < hMax) {
      hPos += stepSize;
    }
  }
  servoH.write(hPos);

  // =====================
  // VERTICAL SERVO (D10)
  // =====================
  if (abs(vertError) > tolerance) {
    if (botAvg > topAvg && vPos > vDown) {
      vPos -= stepSize;
    }
    else if (topAvg > botAvg && vPos < vUp) {
      vPos += stepSize;
    }
  }

  vPos = constrain(vPos, vDown, vUp);
  servoV.write(vPos);

  // =====================
  // VOLTAGE (STABLE READ)
  // =====================
  analogRead(voltagePin); // throw away first read
  delay(5);
  int sensorValue = analogRead(voltagePin);

  // Change this if needed (see note below)
  float voltage = sensorValue * (5.0 / 1023.0) * 5.0;

  // =====================
  // LCD DISPLAY
  // =====================
  lcd.setCursor(0, 0);
  lcd.print("H:");
  lcd.print(hPos);
  lcd.print(" V:");
  lcd.print(vPos);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Volt:");
  lcd.print(voltage, 2);
  lcd.print("V   ");

  delay(10);
}
