#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <math.h>

#define COLOR_ROJO    "\x1b[31m"
#define COLOR_VERDE   "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define COLOR_AZUL    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CIAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

#define RANGO_A 512
#define RANGO_B 1024
#define MAX_PROCS 999999

// Gestión de procesos (lista simple)
int activeProcs[MAX_PROCS];
int activeCount = 0;

void add_proc(int id) {
    if (activeCount < MAX_PROCS) {
        activeProcs[activeCount++] = id;
    }
}

void remove_proc_by_index(int index) {
    activeProcs[index] = activeProcs[activeCount - 1];
    activeCount--;
}

int main() {
    srand(time(NULL));

    float memFisica, a = 1.5, b = 4.5;
    float randomCeroUno = (float)rand() / RAND_MAX;
    float random = (a + randomCeroUno * (b - a));

    // Interacción más natural al inicio
    printf("Hola. Para empezar, necesito saber cuánta RAM (física) vamos a simular (en MB): ");
    scanf("%f", &memFisica);
    int intMemFisica = (int)memFisica;
    
    float memVirtual = (random * memFisica);
    
    float pageSize;
    printf("Perfecto. ¿Y de qué tamaño serán las páginas? (en MB): ");
    scanf("%f", &pageSize);
    int intPageSize = (int)pageSize;

    // Cálculos internos
    int intCantPags = (int)memVirtual / intPageSize; 
    int memRealUtil = intCantPags * intPageSize;
    int framesRamLimit = intMemFisica / intPageSize; 

    // Resumen inicial estilo "charla técnica"
    printf(COLOR_CIAN "\nVale, ya configuré todo. Aquí están los datos:\n");
    printf("Tenemos %d MB de memoria útil en total.\n", memRealUtil);
    printf("De eso, %d MB son RAM real y el resto (%d MB) se va al Swap.\n", intMemFisica, memRealUtil - intMemFisica);
    printf("En total nos salieron %d páginas (o frames).\n", intCantPags);
    printf("Los primeros %d frames son rápidos (RAM), los demás son lentos (Swap).\n", framesRamLimit);
    printf("Empezamos la simulación...\n\n" COLOR_RESET);

    int pages[intCantPags];
    for(int k = 0; k < intCantPags; k++) pages[k] = 0; 

    int memDisponible = intMemFisica; 
    int swapDisponible = memRealUtil - intMemFisica;
    if (swapDisponible < 0) swapDisponible = 0;

    int procId = 1; 

    for(int t = 0; true; t++) {
        
        // Marcador de tiempo simple
        if (t % 2 == 0 || (t >= 30 && t % 5 == 0)) {
            printf(COLOR_AZUL "\n--- Ya van %d segundos ---\n" COLOR_RESET, t);
        }

        // --- 1. CREACIÓN DE PROCESOS ---
        if(t % 2 == 0) {
            int newPsize = rand() % (RANGO_B - RANGO_A + 1) + RANGO_A;
            int pagesNeeded = (newPsize + intPageSize - 1) / intPageSize;
            int memOcupadaReal = pagesNeeded * intPageSize;

            printf(COLOR_VERDE "Voy a intentar crear el Proceso %d.\n" COLOR_RESET, procId);
            printf("Pesa %d MB, así que necesito apartar %d páginas.\n", newPsize, pagesNeeded);

            if (memDisponible < memOcupadaReal) {
                if (swapDisponible + memDisponible < memOcupadaReal) {
                    // Mensaje de error más humano
                    printf(COLOR_ROJO "Uff... malas noticias. No hay espacio en ningún lado.\n");
                    printf("Se llenó la RAM y también el Swap. No cabe nada más.\n");
                    printf("La simulación terminó exitosamente (porque colapsó la memoria).\n" COLOR_RESET);
                    return 0; 
                } else {
                    // Advertencia casual
                    printf(COLOR_AMARILLO "Ojo: La RAM está llena. Voy a tener que tirar esto al Swap.\n" COLOR_RESET);
                    int faltante = memOcupadaReal - memDisponible;
                    memDisponible = 0;
                    swapDisponible -= faltante;
                }
            } else {
                printf("Hay espacio en RAM, así que entra directo.\n");
                memDisponible -= memOcupadaReal;
            }

            int asignadas = 0;
            printf("Listo, asigné las páginas en los índices: ");
            for(int j = 0; j < intCantPags; j++) {
                if(pages[j] == 0) { 
                    pages[j] = procId;
                    printf("%d ", j);
                    asignadas++;
                    if(asignadas == pagesNeeded) break; 
                }
            }
            printf("\n");
            
            add_proc(procId);
            procId++;
        }

        // --- 2. EVENTOS ALEATORIOS (KILL & PAGE FAULT) ---
        if (t >= 30 && t % 5 == 0) {
            
            // A) Matar proceso
            if (activeCount > 0) {
                int victimIndex = rand() % activeCount;
                int victimId = activeProcs[victimIndex];
                
                printf(COLOR_MAGENTA "Le llegó la hora al Proceso %d. Lo estoy finalizando y liberando su memoria...\n" COLOR_RESET, victimId);
                
                for(int j = 0; j < intCantPags; j++) {
                    if (pages[j] == victimId) {
                        pages[j] = 0; 
                        if (j < framesRamLimit) memDisponible += intPageSize;
                        else swapDisponible += intPageSize;
                    }
                }
                remove_proc_by_index(victimIndex);
            }

            // B) Acceso a memoria (Page Fault)
            int randomFrameIndex = rand() % intCantPags;
            int ownerId = pages[randomFrameIndex];

            printf("Simulando acceso a memoria (índice %d)... ", randomFrameIndex);

            if (ownerId == 0) {
                printf("Nada, ese espacio está vacío.\n");
            } else {
                if (randomFrameIndex < framesRamLimit) {
                    printf(COLOR_VERDE "¡Todo bien! Está en RAM (Hit).\n" COLOR_RESET);
                } else {
                    // Page Fault narrado
                    printf(COLOR_ROJO "¡Alerta! Page Fault.\n" COLOR_RESET);
                    printf("Esa página estaba guardada en el Swap (disco).\n");
                    printf(COLOR_AMARILLO "Trayéndola a RAM... (Haciendo un canje aleatorio)\n" COLOR_RESET);

                    int frameVictimaRAM = rand() % framesRamLimit;
                    printf("Saqué la página que estaba en el índice %d (Proceso %d) y la mandé al Swap para hacer espacio.\n", frameVictimaRAM, pages[frameVictimaRAM]);

                    // Intercambio
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