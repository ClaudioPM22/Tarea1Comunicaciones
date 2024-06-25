#include <Wire.h>

const int ledPin = 8; // Pin del LED

volatile char simboloMorse = ' '; // Variable para almacenar el símbolo Morse recibido
volatile bool newData = false; // Indica si se ha recibido un nuevo dato

void setup() {
    Wire.begin(0x08); // Unirse al bus I2C como esclavo con la dirección 0x08
    Wire.onReceive(dataRcv); // Registrar un controlador de eventos para recibir datos
    pinMode(ledPin, OUTPUT); // Configurar el pin del LED como salida
    Serial.begin(9600); // Inicializar el monitor serie
}

void loop() {
    if (newData) {
        Serial.print("Procesando: ");
        Serial.println(simboloMorse);

        if (simboloMorse == '*') {
            // Encender el LED brevemente para un punto
            digitalWrite(ledPin, HIGH);
            delay(100);
            digitalWrite(ledPin, LOW);
        } else if (simboloMorse == '-') {
            // Encender el LED por más tiempo para una raya
            digitalWrite(ledPin, HIGH);
            delay(300);
            digitalWrite(ledPin, LOW);
        }

        newData = false; // Reiniciar la bandera de nuevo dato
    }
}

void dataRcv(int numBytes) {
    while (Wire.available()) { // Leer todos los bytes recibidos
        simboloMorse = Wire.read();
        Serial.print("Recibido: ");
        Serial.println(simboloMorse);
        newData = true; // Indicar que se ha recibido un nuevo dato
    }
}
