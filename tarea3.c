#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));

    float memFisica;
    float x;

    scanf("%f", &memFisica);
    printf("%f \n", memFisica);

    return 0;
}