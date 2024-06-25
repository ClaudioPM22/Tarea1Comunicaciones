#include <Wire.h>

byte i2c_rcv;               // dato recibido del bus I2C
const int ledPin = 8;       // Pin del LED

void setup(){
    Wire.begin(0x08);           // Unirse al bus I2C como esclavo con la dirección 0x08
    Wire.onReceive(dataRcv);    // Registrar un controlador de eventos para recibir datos
    pinMode(ledPin, OUTPUT);    // Configurar el pin del LED como salida
    digitalWrite(ledPin, LOW);  // Asegurarse de que el LED esté apagado inicialmente
}

void loop(){
    // El bucle principal puede estar vacío, ya que las acciones se realizan en dataRcv
}

void dataRcv(int numBytes){
    while (Wire.available()) {  // Leer todos los bytes recibidos
        i2c_rcv = Wire.read();
        if (i2c_rcv == '*') {
            // Encender el LED brevemente para un punto
            digitalWrite(ledPin, HIGH);
            delay(100);
            digitalWrite(ledPin, LOW);
        } else if (i2c_rcv == '-') {
            // Encender el LED por más tiempo para una raya
            digitalWrite(ledPin, HIGH);
            delay(300);
            digitalWrite(ledPin, LOW);
        }
    }
}
