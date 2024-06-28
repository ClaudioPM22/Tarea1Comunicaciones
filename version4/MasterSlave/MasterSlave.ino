#include <Wire.h>

// Constantes y variables para el esclavo
const int ledPin = 8; // Pin del LED
byte pulseState; // Estado del pulso recibido
unsigned long pulseStartTime = 0; // Tiempo cuando el pulso comienza
unsigned long pulseEndTime = 0; // Tiempo cuando el pulso termina
unsigned long lastPulseEndTime = 0; // Tiempo del último pulso

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

// Prototipos de funciones
void receiveEvent(int howMany);
void evaluatePulseDuration(unsigned long duration);
char morseALetra(String morse);
void checkMorseCode(); //Verificar el código Morse

// Constantes y variables para el maestro
const int buttonPin = 2; // Pin del botón
byte buttonState; // Estado del botón

void setup() {
  // Configuración del esclavo
  Serial.begin(9600);
  Wire.begin(0x08); // Unirse al bus I2C como esclavo con dirección 0x08
  Wire.onReceive(receiveEvent); // Registrar el manejador de eventos para datos recibidos

  pinMode(ledPin, OUTPUT); // Configurar el pin del LED como salida
  digitalWrite(ledPin, LOW); // Asegurarse de que el LED esté apagado al inicio

  // Configuración del maestro
  Wire.begin(); // Unirse al bus I2C como maestro

  pinMode(buttonPin, INPUT); // Configurar el pin del botón como entrada
}

void loop() {
  // Lógica del maestro
  // Leer el estado del botón
  buttonState = digitalRead(buttonPin);

  // Enviar el estado del botón al esclavo (0x08)
  Wire.beginTransmission(0x08); // Comenzar la transmisión al esclavo con dirección 0x08
  Wire.write(buttonState); // Enviar el estado del botón
  Wire.endTransmission(); // Finalizar la transmisión

  delay(50); // Pequeño retraso para evitar lecturas muy rápidas

  // Verificar el código Morse si ha pasado más de 1 segundo desde el último pulso
  checkMorseCode();
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
      Serial.print(pulseDuration);
      evaluatePulseDuration(pulseDuration);
      pulseInProgress = false;
      digitalWrite(ledPin, LOW); // Apagar el LED cuando el pulso termina
      lastPulseEndTime = pulseEndTime; // Actualizar el tiempo del último pulso
    }
  }
}

void evaluatePulseDuration(unsigned long duration) {
  if (duration >= 140 && duration <= 260) {
    Serial.print("*");
    morseCode += '*';
  } else if (duration >= 540 && duration <= 660) {
    Serial.print("-");
    morseCode += '-';
  }
}

void checkMorseCode() {
  unsigned long unPressed = millis() - lastPulseEndTime;
  // Verificar si ha pasado más de 1 segundo desde el último pulso
  if (unPressed >= 0 && unPressed <600 && morseCode.length() > 0) { //LETRA
    letra = morseALetra(morseCode);
    Serial.print(letra);
    morseCode = ""; // Reiniciar el código Morse después de la decodificación
  }else if (unPressed >= 600 && unPressed <1100 && morseCode.length() > 0) { //PALABRA
    Serial.print(" ");
  }else if (unPressed >= 1100 && unPressed <= 1700) { //SALTO DE LINEA
    Serial.println("");
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