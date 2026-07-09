#include <Servo.h>

// Pins für das L298N Motor Shield
const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 9;
const int IN4 = 10;

// --- PWM GESCHWINDIGKEITEN FÜR GERADEAUSLAUF ---
// Passe diese beiden Werte an, bis der Roboter geradeaus fährt!
const int SPEED_LINKS = 180;  // Geschwindigkeit linker Motor (IN1/IN2)
const int SPEED_RECHTS = 195; // Geschwindigkeit rechter Motor (IN3/IN4)

// Pins für den HC-SR04 Ultraschallsensor
const int TRIG_PIN = 11;
const int ECHO_PIN = 12;

// Pin und Objekt für den Servo
const int SERVO_PIN = 3;
Servo meinServo;

// Variablen für die Distanzmessung
long dauer;
int distanz;
int distanzRechts;
int distanzLinks;

void setup() {
  // Motor-Pins als Ausgänge definieren
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensor-Pins definieren
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Servo initialisieren und nach vorne ausrichten (90 Grad)
  meinServo.attach(SERVO_PIN);
  meinServo.write(90);
  delay(500);
}

void loop() {
  distanz = messeDistanz();

  if (distanz > 20) {
    vorwaerts();
  } 
  else {
    stopp();
    delay(200);
    rueckwaerts();
    delay(300);
    stopp();
    delay(200);

    umgebungScannen();

    if (distanzRechts >= distanzLinks) {
      dreheRechts();
      delay(200);
    } else {
      dreheLinks();
      delay(200);
    }
    stopp();
    delay(200);
  }
  delay(50);
}

int messeDistanz() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  dauer = pulseIn(ECHO_PIN, HIGH);
  int cm = dauer * 0.034 / 2;
  
  if (cm == 0) cm = 250;
  return cm;
}

void umgebungScannen() {
  meinServo.write(20);
  delay(500);
  distanzRechts = messeDistanz();
  
  meinServo.write(160);
  delay(500);
  distanzLinks = messeDistanz();
  
  meinServo.write(90);
  delay(500);
}

// --- Bewegungsfunktionen mit getrennter PWM-Steuerung ---

void vorwaerts() {
  analogWrite(IN1, SPEED_LINKS);
  analogWrite(IN2, 0);
  analogWrite(IN3, SPEED_RECHTS);
  analogWrite(IN4, 0);
}

void rueckwaerts() {
  analogWrite(IN1, 0);
  analogWrite(IN2, SPEED_LINKS);
  analogWrite(IN3, 0);
  analogWrite(IN4, SPEED_RECHTS);
}

void dreheLinks() {
  // Beim Drehen auf der Stelle nutzen wir die jeweiligen Werte
  analogWrite(IN1, 0);
  analogWrite(IN2, SPEED_LINKS);
  analogWrite(IN3, SPEED_RECHTS);
  analogWrite(IN4, 0);
}

void dreheRechts() {
  analogWrite(IN1, SPEED_LINKS);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, SPEED_RECHTS);
}

void stopp() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}
