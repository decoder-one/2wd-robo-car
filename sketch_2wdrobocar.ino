#include <Servo.h>

// Pins für das L298N Motor Shield
const int IN1 = 5;
const int IN2 = 6;
const int IN3 = 9;
const int IN4 = 10;

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
  delay(500); // Kurz warten, bis der Servo die Position erreicht hat
}

void loop() {
  distanz = messeDistanz();

  // Wenn der Weg frei ist (mehr als 20 cm Platz)
  if (distanz > 20) {
    vorwaerts();
  } 
  // Wenn ein Hindernis im Weg ist
  else {
    stopp();
    delay(200);
    rueckwaerts();
    delay(300);
    stopp();
    delay(200);

    // Umgebung scannen
    umgebungScannen();

    // Entscheiden, wohin die Fahrt geht
    if (distanzRechts >= distanzLinks) {
      dreheRechts();
      delay(200); // Zeit für die Drehung (evtl. anpassen)
    } else {
      dreheLinks();
      delay(200); // Zeit für die Drehung (evtl. anpassen)
    }
    stopp();
    delay(200);
  }
  delay(50); // Kleine Pause zur Stabilisierung
}

// Funktion zur Distanzmessung (Ultraschall)
int messeDistanz() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  dauer = pulseIn(ECHO_PIN, HIGH);
  int cm = dauer * 0.034 / 2;
  
  if (cm == 0) cm = 250; // Fehlerwert abfangen
  return cm;
}

// Servo blickt nach links und rechts und misst die Werte
void umgebungScannen() {
  // Nach rechts schauen
  meinServo.write(20);
  delay(500);
  distanzRechts = messeDistanz();
  
  // Nach links schauen
  meinServo.write(160);
  delay(500);
  distanzLinks = messeDistanz();
  
  // Wieder geradeaus schauen
  meinServo.write(90);
  delay(500);
}

// --- Bewegungsfunktionen für die Motoren ---

void vorwaerts() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void rueckwaerts() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void dreheLinks() {
  // Linkes Rad rückwärts, rechtes Rad vorwärts
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void dreheRechts() {
  // Linkes Rad vorwärts, rechtes Rad rückwärts
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void stopp() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}