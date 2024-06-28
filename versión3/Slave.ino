#include <Wire.h>

const int ledPin = 8; // Pin del LED
byte pulseState; // Estado del pulso recibido
unsigned long pulseStartTime = 0; // Tiempo cuando el pulso comienza
unsigned long pulseEndTime = 0; // Tiempo cuando el pulso termina
bool pulseInProgress = false; // Para rastrear el estado del pulso
String morseCode = "";
char letra = ' ';

// Estructura para almacenar el código Morse y la letra correspondiente
struct Morse {
  const char* code;
  char letter;
};

// Array con el alfabeto Morse
Morse morseDictionary[] = {
  {"*-", 'A'}, {"-***", 'B'}, {"-*-*", 'C'}, {"-**", 'D'}, {"*", 'E'},
  {"**-*", 'F'}, {"--*", 'G'}, {"****", 'H'}, {"**", 'I'}, {"*---", 'J'},
  {"-*-", 'K'}, {"*-**", 'L'}, {"--", 'M'}, {"-*", 'N'}, {"---", 'O'},
  {"*--*", 'P'}, {"--*-", 'Q'}, {"*-*", 'R'}, {"***", 'S'}, {"-", 'T'},
  {"**-", 'U'}, {"***-", 'V'}, {"*--", 'W'}, {"-**-", 'X'}, {"-*--", 'Y'},
  {"--**", 'Z'}
};

void setup() {
  Serial.begin(9600);
  Wire.begin(0x08); // Unirse al bus I2C como esclavo con dirección 0x08
  Wire.onReceive(receiveEvent); // Registrar el manejador de eventos para datos recibidos

  pinMode(ledPin, OUTPUT); // Configurar el pin del LED como salida
  digitalWrite(ledPin, LOW); // Asegurarse de que el LED esté apagado al inicio
}

void loop() {
  // No se necesita lógica en el loop principal
}

// Manejador de eventos para datos recibidos
void receiveEvent(int howMany) {
  while (Wire.available()) {
    pulseState = Wire.read(); // Leer el estado del pulso
    
    if (pulseState == LOW && !pulseInProgress) {
      // Pulso comienza
      pulseStartTime = millis();
      pulseInProgress = true;
      digitalWrite(ledPin, HIGH); // Encender el LED cuando el pulso comienza
    } 
    else if (pulseState == HIGH && pulseInProgress) {
      // Pulso termina
      pulseEndTime = millis();
      unsigned long pulseDuration = pulseEndTime - pulseStartTime;
      evaluatePulseDuration(pulseDuration);
      pulseInProgress = false;
      digitalWrite(ledPin, LOW); // Apagar el LED cuando el pulso termina
    }
  }
}

void evaluatePulseDuration(unsigned long duration) {
  if (duration >= 150 && duration <= 250) {
    Serial.print("*");
    morseCode += '*';
  } else if (duration >= 550 && duration <= 650) {
    Serial.print("-");
    morseCode += '-';
  } else {
    // Pausa larga, decodificar el morseCode actual
    if (morseCode.length() > 0) {
      letra = morseALetra(morseCode);
      Serial.print(" -> ");
      Serial.println(letra);
      morseCode = ""; // Reiniciar el código Morse después de la decodificación
    }
  }
}

char morseALetra(String morse) {
  for (int i = 0; i < sizeof(morseDictionary) / sizeof(Morse); i++) {
    if (morse == morseDictionary[i].code) {
      return morseDictionary[i].letter;
    }
  }
  return '?'; // Devuelve '?' si no se encuentra la secuencia Morse
}
