#include <Wire.h>

const int botonPin = 2;   // Pin del botón
unsigned long tiempoInicio = 0; // Variable para almacenar el tiempo de inicio del estado del botón
bool botonPresionado = false;  // Estado actual del botón
char simboloMorse; // Símbolo Morse a enviar

void setup(){
    Wire.begin(); // Unirse al bus I2C como maestro
    pinMode(botonPin, INPUT); // Configurar el pin del botón como entrada
    Serial.begin(9600); // Inicializar el monitor serie
}

void loop(){
    int estadoBoton = digitalRead(botonPin);

    if (estadoBoton == LOW) { // Botón presionado (asumiendo que el botón está activo en LOW)
        if (!botonPresionado) {
            tiempoInicio = millis();
            botonPresionado = true;
        }
    } else { // Botón no presionado
        if (botonPresionado) {
            unsigned long tiempoPresionado = millis() - tiempoInicio;
            if (tiempoPresionado > 80 && tiempoPresionado <= 200) {
                simboloMorse = '*'; // Punto
            } else if (tiempoPresionado > 200 && tiempoPresionado <= 400) {
                simboloMorse = '-'; // Raya
            } else {
                simboloMorse = ' '; // Tiempo no válido
            }

            if (simboloMorse != ' ') {
                Wire.beginTransmission(0x08); // Iniciar transmisión a esclavo 0x08
                Wire.write(simboloMorse);     // Enviar símbolo Morse
                Wire.endTransmission();       // Terminar transmisión

                Serial.print("Enviado: ");
                Serial.println(simboloMorse);
            }

            botonPresionado = false;
        }
    }
}
