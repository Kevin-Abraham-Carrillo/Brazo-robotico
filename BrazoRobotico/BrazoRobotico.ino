#include <Servo.h> // libreria del servo
// definiendo los pines de los servos los cuales son PWM 
const int servo1Pin = 5;  
const int servo2Pin = 6;   
const int servo3Pin = 9;  
const int servo4Pin = 10;  

// entradas analógicas en modo manual
const int pot1Pin = A0; // pot = potenciómetro 
const int pot2Pin = A1;
const int pot3Pin = A2;
const int pot4Pin = A3;

// entradas analogicas en modo automático 
const int btnSiguientePin = 7; // btn = boton
const int btnAnteriorPin  = 8;  

// selector de modo (manual o automático)
const int modoPin = 2;  // Puede ser switch o jumper

// LEDs indicadores de modo 
const int ledManualPin = 3;  // LED azul
const int ledAutoPin   = 4;  // LED verde

//definicion de servos
Servo servo1, servo2, servo3, servo4;

int angulo1 = -1; // -1 para que pueda actualizar el servo desde donde esta 
int angulo2 = -1;
int angulo3 = -1;
int angulo4 = -1;

// definicion de globales auxiliares
bool estadoAnteriorBtnSiguiente = HIGH;
bool estadoAnteriorBtnAnterior  = HIGH;
unsigned long tiempoUltimoBtnSiguiente = 0;
unsigned long tiempoUltimoBtnAnterior  = 0;

unsigned long tiempoAnterior = 0;
const unsigned long intervalo = 100;  // Cada 100 ms se actualiza
const unsigned long tiempoDebounce = 50;  // Tiempo de antirrebote en milisegundos

void setup() {
  // Pines de entrada y salida
  pinMode(modoPin, INPUT);
  pinMode(btnSiguientePin, INPUT_PULLUP);
  pinMode(btnAnteriorPin, INPUT_PULLUP);
  pinMode(ledManualPin, OUTPUT);
  pinMode(ledAutoPin, OUTPUT);

  // Inicializar comunicación serial
  Serial.begin(9600);

  // Adjuntar servos
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);
}

void mostrarModo() {
  bool Manual = digitalRead(modoPin);

  if (Manual) {
    digitalWrite(ledManualPin, HIGH);
    digitalWrite(ledAutoPin, LOW);
  } else {
    digitalWrite(ledManualPin, LOW);
    digitalWrite(ledAutoPin, HIGH);
  }
}

void loop()  {
  unsigned long tiempoActual = millis();

  // Tareas que se actualizan cada 100 ms
  if (tiempoActual - tiempoAnterior >= intervalo) {
    tiempoAnterior = tiempoActual;

    mostrarModo();
    mostrarPosiciones();  // Se implementará después
  }

  // Modo manual o automático en paralelo
  if (digitalRead(modoPin)) {
    modoManualControl();  // Por implementar
  } else {
    modoAutomaticoControl();  // Por implementar
  }
}

void modoManualControl() {
  int nuevoAngulo1 = map(analogRead(pot1Pin), 0, 1023, 0, 180);
  int nuevoAngulo2 = map(analogRead(pot2Pin), 0, 1023, 0, 180);
  int nuevoAngulo3 = map(analogRead(pot3Pin), 0, 1023, 0, 180);
  int nuevoAngulo4 = map(analogRead(pot4Pin), 0, 1023, 0, 180);

  if (nuevoAngulo1 != angulo1) {
    angulo1 = nuevoAngulo1;
    servo1.write(angulo1);
  }

  if (nuevoAngulo2 != angulo2) {
    angulo2 = nuevoAngulo2;
    servo2.write(angulo2);
  }

  if (nuevoAngulo3 != angulo3) {
    angulo3 = nuevoAngulo3;
    servo3.write(angulo3);
  }

  if (nuevoAngulo4 != angulo4) {
    angulo4 = nuevoAngulo4;
    servo4.write(angulo4);
  }
}

botonEstable() {
bool botonEstable(int pin, bool &estadoAnterior, unsigned long &ultimoTiempo) {
  bool estadoActual = digitalRead(pin);
  unsigned long tiempoActual = millis();

  if (estadoActual != estadoAnterior) {
    if (tiempoActual - ultimoTiempo > tiempoDebounce) {
      estadoAnterior = estadoActual;
      ultimoTiempo = tiempoActual;

      // Detecta flanco descendente (pulsación real con INPUT_PULLUP)
      if (estadoActual == LOW) {
        return true;
      }
    }
  }
  return false;
}


