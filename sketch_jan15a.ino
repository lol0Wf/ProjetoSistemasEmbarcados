#include <ESP8266WiFi.h>
#include <Servo.h>

// Declaração de pinos
const int trigPin = 12;  // D6
const int echoPin = 14;  // D5
const int vibrationPin1 = 5;  // D1
const int vibrationPin2 = 4;  // D2
const int motorIn1 = 15;  // D8
const int motorIn2 = 16;  // D0
const int motorIn3 = 0;   // D3
const int motorIn4 = 2;   // D4
const int servoPin = 13;  // D7

// Constantes
#define SOUND_VELOCITY 0.034
#define MIN_DISTANCE 20

// Variáveis Globais
long duration;
float distanceCm;

// Objeto Servo
Servo servo;

// Configuração do Wi-Fi
const char* ssid = "SEU_SSID";
const char* password = "SUA_SENHA";

// Funções Auxiliares
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();
void stopMotors();
void scanObstacles();
float measureDistance();
void handleVibration();

void setup() {
  // Comunicação Serial
  Serial.begin(115200);

  // Configuração dos pinos
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(vibrationPin1, INPUT);
  pinMode(vibrationPin2, INPUT);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn3, OUTPUT);
  pinMode(motorIn4, OUTPUT);

  // Inicialização do servo motor
  servo.attach(servoPin);
  servo.write(90);  // Centraliza o servo

  // Conexão Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado ao Wi-Fi!");
}

void loop() {
  // Medir distância
  distanceCm = measureDistance();
  Serial.print("Distância: ");
  Serial.println(distanceCm);

  // Reagir a obstáculos
  if (distanceCm < MIN_DISTANCE) {
    stopMotors();
    turnLeft();
  } else {
    moveForward();
  }

  // Monitorar vibração
  handleVibration();

  // Escaneamento com servo
  scanObstacles();

  delay(100);
}

// Função para medir a distância
float measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return (duration * SOUND_VELOCITY) / 2;
}

// Função para controlar os motores
void moveForward() {
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  digitalWrite(motorIn3, HIGH);
  digitalWrite(motorIn4, LOW);
}

void moveBackward() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, HIGH);
}

void turnLeft() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, HIGH);
  digitalWrite(motorIn3, HIGH);
  digitalWrite(motorIn4, LOW);
}

void turnRight() {
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, HIGH);
}

void stopMotors() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  digitalWrite(motorIn3, LOW);
  digitalWrite(motorIn4, LOW);
}

// Função para escanear obstáculos
void scanObstacles() {
  for (int angle = 0; angle <= 180; angle += 30) {
    servo.write(angle);
    delay(500);
    float dist = measureDistance();
    Serial.print("Distância em ");
    Serial.print(angle);
    Serial.print("°: ");
    Serial.println(dist);
  }
  servo.write(90);  // Centraliza novamente
}

// Função para lidar com sensores de vibração
void handleVibration() {
  if (digitalRead(vibrationPin1) == HIGH || digitalRead(vibrationPin2) == HIGH) {
    Serial.println("Vibração detectada!");
    stopMotors();
    delay(1000);
  }
}
