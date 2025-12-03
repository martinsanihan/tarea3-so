#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <math.h>

#define RANGO_A 512
#define RANGO_B 1024

int main() {
    srand(time(NULL));

    float memFisica, a = 1.5, b = 4.5;
    float randomCeroUno = (float)rand() /RAND_MAX;
    float random = (a + randomCeroUno * (b - a));

    printf("Ingresar cantidad de memoria física (RAM) en MB:\n");
    scanf("%f", &memFisica);
    int intMemFisica = (int)memFisica;
    float memVirtual = (random * memFisica);
    int intMemVirtual = (int)memVirtual;

    float pageSize;
    printf("Ingresar tamaño de cada página:\n");
    scanf("%f", &pageSize);
    int intPageSize = (int)pageSize;

    // printf("%f \n%f\n", randomCeroUno, random);
    printf("Memoria Física: %d MB \nMemoria Virtual: %d MB\nTamaño de Páginas: %d MB\n", intMemFisica, intMemVirtual, intPageSize);

    float cantPags = memVirtual / pageSize;
    int intCantPags = (int)cantPags;
    printf("Cantidad de Páginas: %d\n", intCantPags);

    int memDisponible = memFisica;
    float swapDisponible = memVirtual - memFisica;

    bool swap = false, exceso = false;
    int i = 0;
    while(1) {
        int newPsize = rand() % (RANGO_B - RANGO_A + 1) + RANGO_A;
        if (memDisponible < newPsize) {
            if (swapDisponible + memDisponible < newPsize) {
                printf("Memoria insuficiente en el sistema");
                return 0;
            }
            else {
                swapDisponible -= (newPsize - memDisponible);
                memDisponible = 0;
            }
        }
        else {
            memDisponible -= newPsize;
        }

        fflush(stdout);
        i++;
        sleep(1);
    }

    return 0;
}