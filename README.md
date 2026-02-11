# Transmisión inalámbrica portátil de archivos (Raspberry Pi Zero W + NRF24L01+)

## Proyecto final del curso **IE-0527 Ingeniería de Comunicaciones**

### Autores
- Diego José Acosta Obando
- Oscar Fuentes Córdoba
- Carlos Naranjo Arias
- Raúl Andŕes Villaobos Vega

## Descripción general

Este repositorio implementa un enlace **inalámbrico punto a punto** capaz de transmitir un archivo entre dos nodos portátiles usando **NRF24L01+ (banda ISM 2.4 GHz)** y **Raspberry Pi Zero W**.

El sistema está compuesto por dos dispositivos:

- **Transmisor (TX):** lee un archivo local, lo divide en paquetes y lo envía por radiofrecuencia.
- **Receptor (RX):** escucha paquetes, los almacena por número de secuencia, reconstruye el archivo original y lo guarda localmente.

Un **botón físico** activa la ventana de transmisión/recepción, y un **LED** funciona como indicador de actividad.

---

## Funcionalidades principales

- Comunicación inalámbrica punto a punto usando **NRF24L01+**
- Fragmentación de archivo en paquetes de **32 bytes**:
  - `1 byte` número de secuencia
  - `31 bytes` datos
- Reconstrucción del archivo en el receptor usando ordenamiento por secuencia
- Interfaz física simple:
  - Botón = iniciar TX/RX
  - LED = transmisión/recepción activa
- Implementación en **C++**

---

## Hardware

Cada nodo incluye:

- Raspberry Pi Zero W
- Módulo transceptor NRF24L01+ (2.4 GHz)
- Botón (inicio TX o RX)
- LED indicador
- microSD (sistema + almacenamiento de archivos)
- Fuente de alimentación portátil (batería)

---

## Configuración de radio (NRF24L01+)

Ambos nodos utilizan los siguientes parámetros:

- Canal: `76`
- Tasa de datos: `1 Mbps`
- Nivel de potencia: `LOW`
- Dirección lógica: `"1Node"`

---

## Funcionamiento (alto nivel)

1. El transmisor espera pulsación del botón.
2. El transmisor abre una **ventana de transmisión de 5 s** y envía paquetes secuencialmente.
3. El receptor espera pulsación del botón.
4. El receptor abre una **ventana de recepción de 5 s**, recibe paquetes y los guarda por `seq`.
5. El receptor reconstruye el archivo y lo guarda como `archivo_recibido.txt`.