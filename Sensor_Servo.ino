#include <ESP32Servo.h>

const int irSensorPin = 12;   // Pino do sensor IR
const int servoPin = 13;      // Pino do servo motor

Servo myservo; // Objeto Servo

void setup() {
  Serial.begin(115200);       // Inicializar comunicação serial
  myservo.setPeriodHertz(50); // Configurar a frequência do servo motor
  myservo.attach(servoPin);   // Conectar o servo motor ao pino
  pinMode(irSensorPin, INPUT);  // Configurar o pino do sensor como entrada
}

void loop() {
  int irSensorValue = digitalRead(irSensorPin); // Ler o valor do sensor IR

  if (irSensorValue == HIGH) { // Se o objeto for detectado
    Serial.println("Objeto detectado!");

    // Girar o servo para uma posição específica (por exemplo, 90 graus)
    myservo.write(90);
    delay(1000); // Aguardar um segundo

    // Retornar o servo para a posição inicial (por exemplo, 0 grau)
    myservo.write(0);
