// PROYECTO: Control de Brazo Robótico con 4 Servos
// AUTOR: Kevin Abraham Carrillo López 
// CURSO: Electrónica Digital 1
// DESCRIPCIÓN:
// Este programa permite controlar cuatro servomotores en dos modos:
// - Modo manual: mediante potenciómetros.
// - Modo automático: mediante secuencias predefinidas controladas por botones.
// Además, incluye indicación de modo mediante LEDs y reporte de estado vía Monitor Serial.


// ---------- LIBRERÍA ----------
#include <Servo.h>  // Librería para controlar servomotores

// ---------- DEFINICIÓN DE PINES ----------
// Pines PWM para los servomotores
const int servo1Pin = 5;
const int servo2Pin = 6;
const int servo3Pin = 9;
const int servo4Pin = 10;

// Entradas analógicas de los potenciómetros (modo manual)
const int pot1Pin = A0;
const int pot2Pin = A1;
const int pot3Pin = A2;
const int pot4Pin = A3;

// Botones para navegación de secuencia automática
const int btnSiguientePin = 8;
const int btnAnteriorPin  = 7;

// Pin para seleccionar el modo (manual o automático)
const int modoPin = 2;

// Pines para LEDs indicadores de modo
const int ledManualPin = 3;
const int ledAutoPin   = 4;

// ---------- INSTANCIAS DE SERVOS ----------
Servo servo1, servo2, servo3, servo4;

// ---------- VARIABLES GLOBALES ----------
// Variables para almacenar el último ángulo enviado a cada servo
int angulo1 = -1;
int angulo2 = -1;
int angulo3 = -1;
int angulo4 = -1;

// Variables de estado de botones con antirrebote
bool estadoAnteriorBtnSiguiente = HIGH;
bool estadoAnteriorBtnAnterior  = HIGH;
unsigned long tiempoUltimoBtnSiguiente = 0;
unsigned long tiempoUltimoBtnAnterior  = 0;

// Temporizador para impresión lenta en el Monitor Serial
unsigned long tiempoUltimaImpresion = 0;
const unsigned long intervaloImpresion = 500;  // ms

// Estado anterior del modo para detectar cambios
bool estadoModoAnterior = HIGH;

// Matriz de posiciones predefinidas en modo automático
const int posiciones[5][4] = {
  // s1    s2   s3   s4
  { 160,  90,  120, 175 },
  { 90, 120, 100, 110 },
  { 90, 140,  90,  90 },
  {  130,  90, 150,  60 },
  { 160,  60,  120,   5 }
};

int indicePosicion = 0;  // Índice actual de la secuencia automática

// ---------- SETUP INICIAL ----------
void setup() {
  // Configuración de pines
  pinMode(modoPin, INPUT_PULLUP);
  pinMode(btnSiguientePin, INPUT_PULLUP);
  pinMode(btnAnteriorPin, INPUT_PULLUP);
  pinMode(ledManualPin, OUTPUT);
  pinMode(ledAutoPin, OUTPUT);

  Serial.begin(9600);  // Inicializar comunicación serial

  // Asociar cada servo a su pin
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo3.attach(servo3Pin);
  servo4.attach(servo4Pin);

  // Inicializar modo según estado del selector
  if (digitalRead(modoPin) == HIGH) {
    modoManualControl();
  } else {
    moverServos(indicePosicion);
  }

  estadoModoAnterior = digitalRead(modoPin);
}

// ---------- BUCLE PRINCIPAL ----------
void loop() {
  unsigned long tiempoActual = millis();

  // Mostrar información en el Monitor Serial cada 500 ms
  if (tiempoActual - tiempoUltimaImpresion >= intervaloImpresion) {
    tiempoUltimaImpresion = tiempoActual;
    mostrarModo();
    mostrarPosiciones();
    imprimirAngulosSerial();
  }

  // Detectar y reportar cambio de modo
  bool modoActual = digitalRead(modoPin);
  if (modoActual != estadoModoAnterior) {
    estadoModoAnterior = modoActual;
    Serial.println(modoActual ? ">> Cambiado a MODO MANUAL <<" : ">> Cambiado a MODO AUTOMÁTICO <<");
  }

  // Ejecutar el modo correspondiente
  if (modoActual == HIGH) {
    modoManualControl();
  } else {
    modoAutomaticoControl();
  }
}

// ---------- FUNCIONES DE INFORMACIÓN ----------

// Actualiza el estado de los LEDs según el modo activo
void mostrarModo() {
  bool Manual = digitalRead(modoPin) == HIGH;
  digitalWrite(ledManualPin, Manual ? HIGH : LOW);
  digitalWrite(ledAutoPin, Manual ? LOW : HIGH);
}

// Imprime en el Monitor Serial el modo y la posición actual
void mostrarPosiciones() {
  Serial.print("Modo: ");
  Serial.println(digitalRead(modoPin) == HIGH ? "Manual" : "Automático");
  Serial.print("Posición actual (índice): ");
  Serial.println(indicePosicion);
}

// Imprime los ángulos actuales de todos los servos
void imprimirAngulosSerial() {
  Serial.print("Ángulos => ");
  Serial.print("S1: "); Serial.print(angulo1); Serial.print("°  ");
  Serial.print("S2: "); Serial.print(angulo2); Serial.print("°  ");
  Serial.print("S3: "); Serial.print(angulo3); Serial.print("°  ");
  Serial.print("S4: "); Serial.println(angulo4); Serial.print("°");
}

// ---------- FUNCIONES DE CONTROL ----------

// Suaviza la lectura analógica eliminando ruido
int leerSuavizado(int pin) {
  long suma = 0;
  for (int i = 0; i < 5; i++) {
    suma += analogRead(pin);
    delay(1);
  }
  return suma / 5;
}

// Mueve el servo suavemente desde el ángulo actual hasta el objetivo, lo vamos a mover con un servo y lee el estado actual hasta el objetivo y avanza 1 por 1 cada 5 ms
void moverSuave(Servo &servo, int &actual, int objetivo, int paso = 1, int pausa = 5) {
  if (actual == objetivo) return;

  int direccion = (objetivo > actual) ? 1 : -1;

  while (actual != objetivo) {
    actual += direccion * paso;
    if ((direccion > 0 && actual > objetivo) || (direccion < 0 && actual < objetivo)) {
      actual = objetivo;
    }
    servo.write(actual);
    delay(pausa);
  }
}

// Control manual usando potenciómetros con lectura filtrada y movimiento suave
void modoManualControl() {
  int nuevoAngulo1 = map(leerSuavizado(pot1Pin), 0, 1023, 90, 160);
  int nuevoAngulo2 = map(leerSuavizado(pot2Pin), 0, 1023, 40, 140);
  int nuevoAngulo3 = map(leerSuavizado(pot3Pin), 0, 1023, 90, 170);
  int nuevoAngulo4 = map(leerSuavizado(pot4Pin), 0, 1023, 5, 175);

  if (abs(nuevoAngulo1 - angulo1) > 2) moverSuave(servo1, angulo1, nuevoAngulo1);
  if (abs(nuevoAngulo2 - angulo2) > 2) moverSuave(servo2, angulo2, nuevoAngulo2);
  if (abs(nuevoAngulo3 - angulo3) > 2) moverSuave(servo3, angulo3, nuevoAngulo3);
  if (abs(nuevoAngulo4 - angulo4) > 2) moverSuave(servo4, angulo4, nuevoAngulo4);
}

// Control de botones con antirrebote
bool botonEstable(int pin, bool &estadoAnterior, unsigned long &ultimoTiempo) {
  bool estadoActual = digitalRead(pin);
  unsigned long tiempoActual = millis();

  if (estadoActual != estadoAnterior) {
    if ((tiempoActual - ultimoTiempo) > 50) {
      ultimoTiempo = tiempoActual;

      if (estadoAnterior == HIGH && estadoActual == LOW) {
        estadoAnterior = estadoActual;
        return true;
      }
    }
  }

  estadoAnterior = estadoActual;
  return false;
}

// Mueve todos los servos a la posición automática correspondiente
void moverServos(int i) {
  moverSuave(servo1, angulo1, posiciones[i][0]);
  moverSuave(servo2, angulo2, posiciones[i][1]);
  moverSuave(servo3, angulo3, posiciones[i][2]);
  moverSuave(servo4, angulo4, posiciones[i][3]);
}

// Control automático de secuencias mediante botones
void modoAutomaticoControl() {
  if (botonEstable(btnSiguientePin, estadoAnteriorBtnSiguiente, tiempoUltimoBtnSiguiente)) {
    Serial.println(">> Botón SIGUIENTE detectado");
    indicePosicion = (indicePosicion + 1) % 5;  // Rota de 0 a 4 y vuelve a 0
    moverServos(indicePosicion);
  }

  if (botonEstable(btnAnteriorPin, estadoAnteriorBtnAnterior, tiempoUltimoBtnAnterior)) {
    Serial.println(">> Botón ANTERIOR detectado");
    indicePosicion = (indicePosicion - 1 + 5) % 5;  // Rota de 4 a 0 y vuelve a 4
    moverServos(indicePosicion);
  }
}
