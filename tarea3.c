#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <math.h>

// definición de colores para la terminal
#define COLOR_ROJO    "\x1b[31m"
#define COLOR_VERDE   "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define COLOR_AZUL    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CIAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define RANGO_A 512
#define RANGO_B 1024
#define MAX_PROCS 10000

int activeProcs[MAX_PROCS];
int activeCount = 0;

void anadir_proceso(int id) {
    if (activeCount < MAX_PROCS) {
        activeProcs[activeCount] = id;
        activeCount++;
    }
}

void fin_proceso(int index) {
    activeProcs[index] = activeProcs[activeCount - 1];
    activeCount--;
}

int main() {
    srand(time(NULL));

    float a = 1.5, b = 4.5;
    float randomCeroUno = (float)rand() / RAND_MAX;
    float random = (a + randomCeroUno * (b - a));

    int memFisicaInput;
    printf("\n");
    printf("Ingresar cantidad de memoria RAM (en MB): ");
    scanf("%d", &memFisicaInput);
    
    int pageSize;
    printf("Ingresar tamaño de páginas (en MB): ");
    scanf("%d", &pageSize);
    printf("\n");

    // CÁLCULOS CORREGIDOS (Ajuste a enteros y páginas)
    // 1. Calculamos la memoria virtual bruta
    float memVirtualBruta = random * (float)memFisicaInput;

    // 2. Calculamos cuántas páginas totales caben (Virtual Total / Pagina)
    int cantPagsTotal = (int)memVirtualBruta / pageSize;

    // 3. Calculamos cuántas páginas son de RAM (RAM / Pagina)
    int cantFramesRAM = memFisicaInput / pageSize;

    // 4. Calculamos la memoria REAL alineada (sin decimales sueltos)
    int memRealTotal = cantPagsTotal * pageSize;
    int memRealRAM = cantFramesRAM * pageSize;
    int memRealSwap = memRealTotal - memRealRAM;

    printf(COLOR_CIAN "---------------------------------\n");
    printf("Memoria Física (RAM): %d MB\n", memRealRAM);
    printf("Swap: %d MB\n", memRealSwap);
    printf("Páginas: %d\n", cantPagsTotal);
    printf("---------------------------------\n\n" COLOR_RESET);

    // Array con el tamaño exacto de páginas totales
    int pages[cantPagsTotal];
    for(int k = 0; k < cantPagsTotal; k++) pages[k] = 0; 
    
    // Inicializamos contadores con la memoria ALINEADA
    int memDisponible = memRealRAM; 
    int swapDisponible = memRealSwap;
    
    if (swapDisponible < 0) swapDisponible = 0;

    int procId = 1; 

    for(int t = 0; true; t++) {
        
        printf(COLOR_AZUL "%d secs...\n" COLOR_RESET, t);

        if(t % 2 == 0 && t > 0) {
            int newPsize = rand() % (RANGO_B - RANGO_A + 1) + RANGO_A;
            
            // Calculamos cuántas páginas ocupa este proceso (techo)
            int procPages = (newPsize + pageSize - 1) / pageSize;
            // El espacio que realmente ocupará en memoria (múltiplo de página)
            int memNecesaria = procPages * pageSize;

            printf(COLOR_VERDE "Creando proceso %d\n" COLOR_RESET, procId);
            printf("Se necesitan %d páginas para el proceso\n", procPages);

            // Verificamos usando memNecesaria (espacio real)
            if (memDisponible < memNecesaria) {
                if (swapDisponible + memDisponible < memNecesaria) {
                    printf(COLOR_ROJO "No queda espacio disponible. Terminando ejecución...\n" COLOR_RESET);
                    return 0; 
                } else {
                    printf(COLOR_AMARILLO "Memoria RAM llena. Utilizando swap.\n" COLOR_RESET);
                    int faltante = memNecesaria - memDisponible;
                    
                    // Vaciamos RAM y restamos el remanente al Swap
                    memDisponible = 0;
                    swapDisponible -= faltante;
                }
            } else {
                printf("Proceso cargado en la RAM.\n");
                memDisponible -= memNecesaria;
            }

            int asignadas = 0;
            printf("Páginas asignadas: ");
            for(int j = 0; j < cantPagsTotal; j++) {
                if(pages[j] == 0) { 
                    pages[j] = procId;
                    printf("%d ", j);
                    asignadas++;
                    if(asignadas == procPages) break; 
                }
            }
            printf("\n");
            
            anadir_proceso(procId);
            procId++;
        }

        if (t >= 30 && t % 5 == 0) {
            
            if (activeCount > 0) {
                int victimIndex = rand() % activeCount;
                int victimId = activeProcs[victimIndex];
                
                printf(COLOR_MAGENTA "Finalizando proceso %d...\n" COLOR_RESET, victimId);
                
                for(int j = 0; j < cantPagsTotal; j++) {
                    if (pages[j] == victimId) {
                        pages[j] = 0; 
                        // Devolvemos memoria al contador correcto
                        if (j < cantFramesRAM) memDisponible += pageSize;
                        else swapDisponible += pageSize;
                    }
                }
                fin_proceso(victimIndex);
            }

            int randomFrameIndex = rand() % cantPagsTotal;
            int ownerId = pages[randomFrameIndex];

            printf("Acceso a memoria (índice %d)... ", randomFrameIndex);

            if (ownerId == 0) {
                printf("Espacio vacío.\n");
            } else {
                if (randomFrameIndex < cantFramesRAM) {
                    printf(COLOR_VERDE "El proceso ya está en RAM.\n" COLOR_RESET);
                } else {
                    printf(COLOR_ROJO "Fallo de Página.\n" COLOR_RESET);
                    printf("Accediendo al disco (swapping)\n");
                    printf(COLOR_AMARILLO "Política de reemplazo: Aleatorio.\n" COLOR_RESET);

                    // Swap solo ocurre con páginas dentro del rango de RAM
                    int frameVictimaRAM = rand() % cantFramesRAM;
                    printf("Página que estaba en el índice %d (Proceso %d) enviada al Swap.\n", frameVictimaRAM, pages[frameVictimaRAM]);

                    int temp = pages[frameVictimaRAM];
                    pages[frameVictimaRAM] = pages[randomFrameIndex]; 
                    pages[randomFrameIndex] = temp; 
                }
            }
        }

        fflush(stdout);
        sleep(1);
    }

    return 0;
}