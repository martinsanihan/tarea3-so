# Simulador de Gestión de Memoria (Paginación)

Este proyecto es una simulación en C de cómo un Sistema Operativo gestiona la memoria utilizando la técnica de **paginación**. El programa simula la asignación de memoria RAM, el uso de memoria secundaria (Swap), y eventos como fallos de página (Page Faults).

## 📋 Descripción General

El programa actúa como un administrador de memoria que recibe procesos generados aleatoriamente. Su objetivo es asignarles espacio, priorizando siempre la memoria física (RAM). Si la RAM se llena, el sistema utiliza la memoria Swap (intercambio). Si ambas memorias se llenan, la simulación termina exitosamente.

Además, el simulador incluye eventos aleatorios como la finalización de procesos y el acceso a direcciones de memoria virtual para demostrar el funcionamiento del **Swapping**.

## 🚀 Instrucciones de Ejecución

El código está diseñado para compilarse y ejecutarse en un entorno Linux/Unix.

### 1. Compilación
Utiliza `gcc` para compilar el código fuente. Se recomienda enlazar la librería matemática (`-lm`) por si el compilador lo requiere:

```bash
gcc tarea3_human.c -o sim_memoria -lm