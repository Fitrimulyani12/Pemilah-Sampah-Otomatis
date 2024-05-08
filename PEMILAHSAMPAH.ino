// Include Library Required
#include "Servo.h"
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#include "ArduinoJson.h"
#include "SoftwareSerial.h"

// DESCRIPTION VARIABLE Required
#define LED_RED_ORGANIK 2
#define LED_GREEN_ORGANIK 3
#define LED_RED_UNORGANIK 4
#define LED_GREEN_UNORGANIK 5
#define TRIG_PIN_ORGANIK 8
#define ECHO_PIN_ORGANIK 9
#define TRIG_PIN_UNORGANIK 10
#define ECHO_PIN_UNORGANIK 11
const int BUZZER_ORGANIK = 6;
const int BUZZER_UNORGANIK = 7;
const int Servo_Open = 180;
const int Servo_Close = 0;
int Pro_IND = A1;
int Pro_KAP = A2;
int INFRA = A3;
long duration_organik;
long cm_organik;
long duration_unorganik;
long cm_unorganik;
String data;
String calculate;
char c;
// Initialize VARIABLE Required
Servo servoOrganik;
Servo servoUnorganik;
LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial nodemcu(A4,A5);
// SETUP
void setup() {
  // PORT SERIAL MONITOR
  Serial.begin(115200);
  nodemcu.begin(115200);
  // PIN MODE
  pinMode(BUZZER_ORGANIK, OUTPUT);
  pinMode(BUZZER_UNORGANIK, OUTPUT);
  pinMode(LED_RED_ORGANIK, OUTPUT);
  pinMode(LED_GREEN_ORGANIK, OUTPUT);
  pinMode(LED_RED_UNORGANIK, OUTPUT);
  pinMode(LED_GREEN_UNORGANIK, OUTPUT);
  pinMode(TRIG_PIN_ORGANIK, OUTPUT);
  pinMode(ECHO_PIN_ORGANIK, INPUT);
  pinMode(TRIG_PIN_UNORGANIK, OUTPUT);
  pinMode(ECHO_PIN_UNORGANIK, INPUT);
  pinMode(Pro_IND, INPUT);
  pinMode(Pro_KAP, INPUT);
  pinMode(INFRA, INPUT);
  // RESET
  digitalWrite(LED_RED_ORGANIK, LOW);
  digitalWrite(LED_GREEN_ORGANIK, LOW);
  digitalWrite(LED_RED_UNORGANIK, LOW);
  digitalWrite(LED_GREEN_UNORGANIK, LOW);
  servoOrganik.attach(12);
  servoOrganik.write(Servo_Close);
  servoUnorganik.attach(13);
  servoUnorganik.write(Servo_Close);
  lcd.init();
  lcd.backlight();
  // Delay
  delay(10);
}
// START SYSTEMS (LCD)
void welcome(){
  Serial.print("Sistem Pemilah Sampah Aktif");
  Serial.println();
  lcd.setCursor(0, 0);
  lcd.print(" Sistem Pemilah");
  lcd.setCursor(0, 1);
  lcd.print("     Sampah     ");
}
// CEK TINGGI SAMPAH ORGANIK
void cek_tinggi_sampah_organik() {
  // ACTIVE SENSOR HC-S04
  digitalWrite(TRIG_PIN_ORGANIK, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN_ORGANIK, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_ORGANIK, LOW);
  pinMode(ECHO_PIN_ORGANIK, INPUT);
  duration_organik = pulseIn(ECHO_PIN_ORGANIK, HIGH);
  // CONVERT TO cm
  cm_organik = (duration_organik / 2) / 29.1;
  Serial.print("Ketinggian Sampah Organik : ");
  Serial.print(cm_organik);
  Serial.print("cm");
  Serial.println();
  // CONDITIONS
  if (cm_organik < 2) {
    digitalWrite(LED_RED_ORGANIK, HIGH);
    digitalWrite(LED_GREEN_ORGANIK, LOW);
    tone(BUZZER_ORGANIK, 1000);
    lcd.setCursor(0, 0);
    lcd.print("Sampah Organik");
    lcd.setCursor(0, 1);
    lcd.print("   Penuh");
    nodemcu.print("NOTIFIKASI SAMPAH ORGANIK PENUH");
  } else if (cm_organik == 3 || cm_organik <= 6) {
    digitalWrite(LED_RED_ORGANIK, LOW);
    digitalWrite(LED_GREEN_ORGANIK, HIGH);
    noTone(BUZZER_ORGANIK);
    lcd.setCursor(0, 0);
    lcd.print("Tinggi Sampah");
    lcd.setCursor(0, 1);
    lcd.print("Organik ");
    lcd.print(cm_organik);
    lcd.print(" cm");
    nodemcu.print("NOTIFIKASI SAMPAH ORGANIK HAMPIR PENUH");
  } else if (cm_organik >= 6) {
    digitalWrite(LED_RED_ORGANIK, LOW);
    digitalWrite(LED_GREEN_ORGANIK, HIGH);
    noTone(BUZZER_ORGANIK);
    lcd.setCursor(0, 0);
    lcd.print("Tinggi Sampah");
    lcd.setCursor(0, 1);
    lcd.print("Organik ");
    lcd.print(cm_organik);
    lcd.print(" cm");
  } else if (cm_organik > 26) {
    digitalWrite(LED_RED_ORGANIK, LOW);
    digitalWrite(LED_GREEN_ORGANIK, HIGH);
    noTone(BUZZER_ORGANIK);
    lcd.setCursor(0, 0);
    lcd.print("Tinggi Sampah");
    lcd.setCursor(0, 1);
    lcd.print("Organik ");
    lcd.print(cm_organik);
    lcd.print(" cm");
    nodemcu.print("NOTIFIKASI SAMPAH ORGANIK KOSONG");
  }
}
// CEK TINGGI SAMPAH UNORGANIK
void cek_tinggi_sampah_unorganik() {
  // ACTIVE SENSOR HC-S04
  digitalWrite(TRIG_PIN_UNORGANIK, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG_PIN_UNORGANIK, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN_UNORGANIK, LOW);
  pinMode(ECHO_PIN_UNORGANIK, INPUT);
  duration_unorganik = pulseIn(ECHO_PIN_UNORGANIK, HIGH);
  // CONVERT TO cm
  cm_unorganik = (duration_unorganik / 2) / 29.1;
  Serial.print("Ketinggian Sampah UnOrganik : ");
  Serial.print(cm_unorganik);
  Serial.print("cm");
  Serial.println();
  // CONDITIONS
  if (cm_unorganik < 3) {
    digitalWrite(LED_RED_UNORGANIK, HIGH);
    digitalWrite(LED_GREEN_UNORGANIK, LOW);
    tone(BUZZER_UNORGANIK, 1000);
    lcd.setCursor(0, 0);
    lcd.print("Sampah UnOrganik");
    lcd.setCursor(0, 1);
    lcd.print("   Penuh");
    nodemcu.print("NOTIFIKASI SAMPAH UNORGANIK PENUH");
  } else if (cm_unorganik == 3 || cm_unorganik <= 6) {
    digitalWrite(LED_RED_UNORGANIK, LOW);
    digitalWrite(LED_GREEN_UNORGANIK, HIGH);
    noTone(BUZZER_UNORGANIK);
    lcd.setCursor(0, 0);
    lcd.print("Tinggi Sampah");
    lcd.setCursor(0, 1);
    lcd.print("UnOrganik ");
    lcd.print(cm_unorganik);
    lcd.print(" cm");
    nodemcu.print("NOTIFIKASI SAMPAH UNORGANIK HAMPIR PENUH");
  } else if (cm_unorganik >= 6) {
    digitalWrite(LED_RED_UNORGANIK, LOW);
    digitalWrite(LED_GREEN_UNORGANIK, HIGH);
    noTone(BUZZER_UNORGANIK);
    lcd.setCursor(0, 0);
    lcd.print("Tinggi Sampah");
    lcd.setCursor(0, 1);
    lcd.print("UnOrganik ");
    lcd.print(cm_unorganik);
    lcd.print(" cm");
  } else if (cm_unorganik > 26) {
    digitalWrite(LED_RED_UNORGANIK, LOW);
    digitalWrite(LED_GREEN_UNORGANIK, HIGH);
    noTone(BUZZER_UNORGANIK);
    lcd.setCursor(0, 0);
    lcd.print("Tinggi Sampah");
    lcd.setCursor(0, 1);
    lcd.print("UnOrganik ");
    lcd.print(cm_unorganik);
    lcd.print(" cm");
    nodemcu.print("NOTIFIKASI SAMPAH UNORGANIK KOSONG");
  }
}
void loop() {
  // DESCRIPTION SENSOR PROXIMITY AND INFRA RED
  int sensor_Inductive = digitalRead(Pro_IND);
  int sensor_Capacitive = digitalRead(Pro_KAP);
  int sensor_Infrared = digitalRead(INFRA);
  // CONDITIONS IF SENSOR PROXIMITY AND INFRA RED OFF (NO DETECTS)
  if (sensor_Capacitive == 0 && sensor_Inductive == 1 && sensor_Infrared == 1) {
    // VOID welcome()
    welcome();
    delay(1500);
    lcd.clear();
    // VOID cek_tinggi_sampah_organik()
    cek_tinggi_sampah_organik();
    delay(1500);
    lcd.clear();
    // VOID cek_tinggi_sampah_unorganik()
    cek_tinggi_sampah_unorganik();
    delay(1500);
    lcd.clear();
    // CONDITIONS
  } else if (sensor_Inductive == 0 || sensor_Capacitive == 1 && sensor_Inductive == 0 && sensor_Infrared == 0 || sensor_Inductive == 0 && sensor_Infrared == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Mendeteksi");
    lcd.setCursor(0, 1);
    lcd.print("Sampah");
    delay(1000);
    Serial.print("Sampah UnOrganik Terdeteksi");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sampah");
    lcd.setCursor(0, 1);
    lcd.print("UnOrganik");
    servoUnorganik.write(Servo_Open);
    delay(3000);
    servoUnorganik.write(Servo_Close);
    delay(500);
    lcd.clear();
  } else if (sensor_Capacitive == 1 || sensor_Capacitive == 1 && sensor_Inductive == 1 || sensor_Capacitive == 1 && sensor_Infrared == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Mendeteksi");
    lcd.setCursor(0, 1);
    lcd.print("Sampah");
    delay(1000);
    Serial.print("Sampah Organik Terdeteksi");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sampah");
    lcd.setCursor(0, 1);
    lcd.print("Organik");
    servoOrganik.write(Servo_Open);
    delay(3000);
    servoOrganik.write(Servo_Close);
    delay(500);
    lcd.clear();
  } else if (sensor_Infrared == 0) {
    lcd.setCursor(0, 0);
    lcd.print("Mendeteksi");
    lcd.setCursor(0, 1);
    lcd.print("Sampah");
    delay(1000);
    Serial.print("Sampah UnOrganik Terdeteksi");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sampah");
    lcd.setCursor(0, 1);
    lcd.print("UnOrganik");
    servoUnorganik.write(Servo_Open);
    delay(3000);
    servoUnorganik.write(Servo_Close);
    delay(500);
    lcd.clear();
  }
  else {
    // VOID welcome()
    welcome();
    delay(3000);
    lcd.clear();
    // VOID cek_tinggi_sampah_organik()
    cek_tinggi_sampah_organik();
    delay(3000);
    lcd.clear();
    // VOID cek_tinggi_sampah_unorganik()
    cek_tinggi_sampah_unorganik();
    delay(3000);
    lcd.clear();
  }
}
