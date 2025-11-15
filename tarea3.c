#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RANGO_A 512
#define RANGO_B 1024

int main() {
    srand(time(NULL));

    float memFisica, a = 1.5, b = 4.5;
    float randomCeroUno = (float)rand() /RAND_MAX;
    float random = (a + randomCeroUno * (b - a));

    printf("Cuánta Memoria Física (RAM) Tienes?\n");
    scanf("%f", &memFisica);
    int intMemFisica = (int)memFisica;
    float memVirtual = (random * memFisica);

    float pageSize;
    printf("Tamaño de Páginas?\n");
    scanf("%f", &pageSize);
    int intPageSize = (int)pageSize;

    // printf("%f \n%f\n", randomCeroUno, random);
    printf("Memoria Física: %d \nMemoria Virtual: %f\nTamaño de Páginas: %d\n", intMemFisica, memVirtual, intPageSize);

    float cantPags = memVirtual / pageSize;
    printf("Cantidad de Páginas: %f\n", cantPags);

    int i = 0;
    while(1) {
        
    }

    return 0;
}