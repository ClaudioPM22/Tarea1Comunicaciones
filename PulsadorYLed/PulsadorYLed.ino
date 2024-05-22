const int botonPin = 2;   // Pin del botón
const int ledPin = 8;    // Pin del LED
unsigned long tiempoInicio = 0; // Inicializar el tiempo de inicio del estado del botón
unsigned long tiempoPresionado = 0; // Variable para almacenar el tiempo total que se ha mantenido presionado el botón

void setup() {
  pinMode(botonPin, INPUT); // Configurar el pin del botón como entrada
  pinMode(ledPin, OUTPUT);  // Configurar el pin del LED como salida
  Serial.begin(9600);       // Iniciar comunicación serial a 9600 baudios
}

void loop() {
  // Leer el estado del botón
  int estadoBoton = digitalRead(botonPin);
  
  // Si se detecta que el botón está presionado
  if (estadoBoton == HIGH) {
    // Si es la primera vez que se detecta el botón presionado
    if (tiempoInicio == 0) {
      tiempoInicio = millis(); // Almacenar el tiempo de inicio del estado del botón
    }
    // Encender el LED
    digitalWrite(ledPin, HIGH);
  } else { // Si el botón no está presionado
    // Si el botón estaba presionado anteriormente
    if (tiempoInicio != 0) {
      // Calcular el tiempo total que se ha mantenido presionado el botón
      tiempoPresionado = millis() - tiempoInicio;
      // Imprimir el tiempo que se mantuvo presionado el botón
      //Serial.print("Tiempo presionado: ");
      //Serial.print(tiempoPresionado);
      //Serial.println(" ms");
      
      // Ver si es punto o es raya
      if(tiempoPresionado <= 200 && tiempoPresionado > 80) {
        Serial.print("*");
      } else if(tiempoPresionado <= 600 && tiempoPresionado > 200) {
        Serial.print("-");
      } else if(tiempoPresionado <=1400){
        Serial.println("\nPalabra nueva: \n");  
        /*Mi idea es guardar cada codigo hasta el punto en el que se convierta en otra letra, esto se almacena en un arreglo y mandarla a una funcion que la convierta en una letra*/
      }
      
      // Reiniciar el tiempo de inicio del estado del botón
      tiempoInicio = 0;
    }
    // Apagar el LED
    digitalWrite(ledPin, LOW);
  }
}
