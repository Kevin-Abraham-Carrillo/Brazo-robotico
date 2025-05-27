# Brazo Robótico – Proyecto 2 de Electrónica Digital

Este proyecto simula y controla un brazo robótico usando Arduino, cumpliendo los requisitos del curso IE2024.

## 🔧 Modos de funcionamiento

- **Modo Manual:** Control de 4 servos mediante potenciómetros con lectura suavizada.
- **Modo Automático:** Rutina de 5 posiciones predefinidas, controladas por dos botones (siguiente / anterior), con movimiento progresivo.

## 💡 Características

- Selección de modo mediante interruptor físico.
- Indicadores físicos de modo mediante LEDs (manual / automático).
- Antirrebote implementado en botones físicos con verificación de flanco.
- Visualización de ángulos de los servomotores en consola serial.
- Movimiento progresivo entre posiciones (función personalizada `moverSuave`).
- Lectura filtrada de potenciómetros (función personalizada `leerSuavizado`).
- Código organizado y completamente comentado línea por línea.

## 📂 Archivos principales

- `BrazoRobotico.ino`: Código fuente del sistema (comentado y validado).
- `README.md`: Descripción del proyecto.

## 🎥 Videos de funcionamiento

- [Video de funcionamiento físico](https://youtu.be/NTvcPJtqtIA)
- [Explicación del código y control](https://youtu.be/bxls8XgvWuc)

## 🔗 Simulación Wokwi

Puedes ver una verificación funcional completa del sistema aquí:
[https://wokwi.com/projects/430150601346971649](https://wokwi.com/projects/430150601346971649)

## 🧠 Integrantes del equipo

- Kevin Abraham Carrillo López

