#include "DHT.h"

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

int buzzerPin = 8;

int led_green = 13;
int led_yellow = 12;
int led_red = 11;

int command;

int s_humidity = 50;
int s_temp = 30;

int ldr_pin = A5;

void setup() {
  Serial.begin(9600);

  dht.begin();

  pinMode(led_green, OUTPUT);
  pinMode(led_yellow, OUTPUT);
  pinMode(led_red, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

}

void loop() {
  tone(buzzerPin, 50);   // low frequency
  delay(200);             // short beep

  noTone(buzzerPin);
  delay(2000);            // long silence

//   int lightValue = analogRead(ldr_pin);
//   Serial.print("Light Intensity: ");
//   Serial.println(lightValue);

//   // Condition: It is DARK 
//   if(lightValue < 500){
//     digitalWrite(led_red, HIGH); // Turn ON Red LED
//     digitalWrite(led_green, LOW); // Turn OFF Green LED
//     Serial.print("-> System Status: NIGHT MODE (Red ON)");
//   }
//   else {
//     digitalWrite(led_red, LOW); // Turn OFF Red LED
//     digitalWrite(led_green, HIGH); // Turn ON Green LED
//     Serial.print("-> System Status: DAY MODE (Green ON) ");
//   }
// Serial.println("----------------------------------------------");

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (humidity > s_humidity && temperature > s_temp) {
    digitalWrite(led_red, HIGH);
    digitalWrite(led_yellow, HIGH);
    digitalWrite(led_green, LOW);
    Serial.print("High temp: ");
    Serial.print(temperature);
    Serial.print(" High Humidity: ");
    Serial.print(humidity);
    Serial.println();

  } else if (humidity > s_humidity) {
    digitalWrite(led_red, LOW);
    digitalWrite(led_yellow, HIGH);
    digitalWrite(led_green, LOW);
    Serial.print("High Humidity: ");
    Serial.print(humidity);
    Serial.println();
  } else if (temperature > s_temp) {
    digitalWrite(led_red, HIGH);
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_green, LOW);
    Serial.print("High Temp: ");
    Serial.print(temperature);
    Serial.println();
  } else {
    digitalWrite(led_red, LOW);
    digitalWrite(led_yellow, LOW);
    digitalWrite(led_green, HIGH);
    Serial.print("You are good. Temp: ");
    Serial.print(temperature);
    Serial.print(" + Humidity: ");
    Serial.print(humidity);
    Serial.println();
  }

  delay(2000);


  // Serial.print("Humidity: ");
  // Serial.print(humidity);
  // Serial.print("%  Temperature: ");
  // Serial.print(temperature);
  // Serial.println(" C");




  // if (Serial.available() > 0 ) {
  //   command = Serial.read(); 
  //   if (command == '1') {
  //     digitalWrite(led_green, HIGH);
  //     Serial.println("GREEN LED IS ON");
  //     // delay(1000);
  //   }
  //   if (command == '0') {
  //     digitalWrite(led_green, LOW);
  //     Serial.println("GREEN LED IS OFF");
  //     // delay(1000);
  //   }
  // }

  // digitalWrite(led_green, HIGH);
  // delay(300);


  // digitalWrite(led_yellow, HIGH);
  // delay(300);

  // digitalWrite(led_red, HIGH);
  // delay(300);


  // digitalWrite(led_green, LOW);
  // delay(300);


  // digitalWrite(led_yellow, LOW);
  // delay(300);

  // digitalWrite(led_red, LOW);
  // delay(300);
  
}