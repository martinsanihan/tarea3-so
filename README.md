# Tarea 3 Sistemas Operativos: Paginación

En esta tarea se pidió aplicar un gestor de memoria utilizando la paginación. Se simula un dispositivo con su memoria física (RAM), memoria virtual, tabla de páginas, y eventos como finalización de procesos y accesos a memoria.

## Descripción

Cada 2 segundos se crean procesos (a nivel lógico) de tamaño aleatorio, en un rango predefinido, a los cuales se les asigna las páginas en memoria, cada iteración se verifica que aun quede memoria disponible. Luego de 30 segundos de ejecución, pueden suceder eventos, un acceso a memoria y la finalización de otro proceso, teniendo como consecuencia fallos de página y liberación de memoria. La ejecución del el programa termina si ya no queda memoria disponible, ni principal ni secundaria.

## Ejecución

Para ejecutar el código se debe compilar en un entorno Unix. En el directorio donde está el código, se debe compilar y ejecutar con los siguientes comandos:

```bash
gcc tarea3.c -o t3
./t3
```