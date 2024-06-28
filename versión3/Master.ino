#include <Wire.h>

const int buttonPin = 2; // Pin del botón
byte buttonState; // Estado del botón

void setup() {
  Wire.begin(); // unirse al bus I2C como maestro

  pinMode(buttonPin, INPUT); // configurar el pin del botón como entrada
}

void loop() {
  // Leer el estado del botón
  buttonState = digitalRead(buttonPin);

  // Enviar el estado del botón al esclavo (0x08)
  Wire.beginTransmission(0x08); // comenzar la transmisión al esclavo con dirección 0x08
  Wire.write(buttonState); // enviar el estado del botón
  Wire.endTransmission(); // finalizar la transmisión

  delay(50); // pequeño retraso para evitar lecturas muy rápidas
}