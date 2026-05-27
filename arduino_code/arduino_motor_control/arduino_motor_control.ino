#include <SoftwareSerial.h>

// ESP32 communicates via pins 2 and 3
SoftwareSerial espSerial(2, 3);

// Motor pins
int IN1 = 6;
int IN2 = 7;
int IN3 = 8;
int IN4 = 9;
int ENA = 5;
int ENB = 10;

void setup() {
  Serial.begin(9600);
  espSerial.begin(9600);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotors();
  Serial.println("Arduino Ready!");
}

void loop() {
  if (espSerial.available() > 0) {
    String data = espSerial.readStringUntil('\n');
    Serial.println("Received: " + data);
    parseAndMove(data);
  }
}

void parseAndMove(String data) {
  char cmd = data[0];
  int spd = 0;

  if (cmd != 'S') {
    spd = data.substring(1).toInt();
    spd = constrain(spd, 0, 255);
  }

  if (cmd == 'F') {
    moveForward(spd);
  } else if (cmd == 'L') {
    turnLeft(spd);
  } else if (cmd == 'R') {
    turnRight(spd);
  } else if (cmd == 'S') {
    stopMotors();
  }
}

void moveForward(int spd) {
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Forward: " + String(spd));
}

void turnLeft(int spd) {
  analogWrite(ENA, spd);
  analogWrite(ENB, spd);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("