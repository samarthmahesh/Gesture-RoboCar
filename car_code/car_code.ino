#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

/* ================= MOTOR PINS ================= */

#define AIN1 26
#define AIN2 25
#define PWMA 33

#define BIN1 14
#define BIN2 12
#define PWMB 13

#define STDBY 27

/* ================= PWM CHANNELS ================= */

#define PWM_CH_A 0
#define PWM_CH_B 1

/* ================= WIFI ================= */

const char* ssid     = "NAME_OF_AP";
const char* password = "PASSWORD_AP";

IPAddress local_IP(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiUDP udp;
const int udpPort = 4210;
char incomingPacket[255];

/* ================= CONTROL STATE ================= */

int lastCommand = -1;
unsigned long lastPacketTime = 0;
const unsigned long CONNECTION_TIMEOUT = 500;

/* ================= MOTOR CONTROL ================= */

void runMotor(int motor, int spd, int dir) {

  digitalWrite(STDBY, HIGH);

  bool dirPin1 = LOW;
  bool dirPin2 = HIGH;

  if (dir == 1) {
    dirPin1 = HIGH;
    dirPin2 = LOW;
  }

  if (motor == 0) {
    digitalWrite(AIN1, dirPin1);
    digitalWrite(AIN2, dirPin2);
    ledcWrite(PWM_CH_A, spd);
  } else {
    digitalWrite(BIN1, dirPin1);
    digitalWrite(BIN2, dirPin2);
    ledcWrite(PWM_CH_B, spd);
  }
}

void forward() { runMotor(0, 255, 0); runMotor(1, 255, 0); }
void back()    { runMotor(0, 255, 1); runMotor(1, 255, 1); }
void left()    { runMotor(0,  80, 0); runMotor(1, 255, 0); }
void right()   { runMotor(0, 255, 0); runMotor(1,  80, 0); }

void stopMotors() {
  digitalWrite(STDBY, LOW);
}

/* ================= SETUP ================= */

void setup() {

  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid, password);

  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  udp.begin(udpPort);
  Serial.println("UDP server started");

  pinMode(AIN1,  OUTPUT);
  pinMode(AIN2,  OUTPUT);
  pinMode(PWMA,  OUTPUT);
  pinMode(BIN1,  OUTPUT);
  pinMode(BIN2,  OUTPUT);
  pinMode(PWMB,  OUTPUT);
  pinMode(STDBY, OUTPUT);

  ledcSetup(PWM_CH_A, 1000, 8);
  ledcAttachPin(PWMA, PWM_CH_A);

  ledcSetup(PWM_CH_B, 1000, 8);
  ledcAttachPin(PWMB, PWM_CH_B);

  stopMotors();
}

/* ================= LOOP ================= */

void loop() {

  int packetSize = udp.parsePacket();

  if (packetSize > 0) {

    int len = udp.read(incomingPacket, sizeof(incomingPacket) - 1);
    if (len > 0) incomingPacket[len] = '\0';

    lastCommand     = atoi(incomingPacket);
    lastPacketTime  = millis();

    Serial.print("Command: ");
    Serial.println(lastCommand);

    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.print("ACK");
    udp.endPacket();
  }

  if (millis() - lastPacketTime > CONNECTION_TIMEOUT) {
    if (lastCommand != -1) {
      Serial.println("Connection lost -> STOP");
      lastCommand = -1;
      stopMotors();
    }
    return;
  }

  switch (lastCommand) {
    case 0:  stopMotors(); break;
    case 1:  forward();    break;
    case 2:  back();       break;
    case 3:  left();       break;
    case 4:  right();      break;
    default: stopMotors();
  }
}
