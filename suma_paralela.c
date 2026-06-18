#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 100000000 // 100 millones para asegurar carga de trabajo

int main() {
    // Asignación de memoria dinámica para evitar desbordamiento de pila (stack overflow)
    long long *arr = (long long *)malloc(ARRAY_SIZE * sizeof(long long));
    if (arr == NULL) {
        printf("Error al asignar memoria.\n");
        return 1;
    }

    // Inicialización del arreglo (secuencial)
    for (long long i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = 1; 
    }

    // Hilos requeridos por el mandato
    int threads[] = {1, 2, 4, 8, 16};
    int num_test_cases = 5;

    printf("Hilos\tTiempo (s)\t\tSuma Total\n");
    printf("--------------------------------------------------\n");

    // Ejecución y medición para cada configuración de hilos
    for (int t = 0; t < num_test_cases; t++) {
        int num_threads = threads[t];
        omp_set_num_threads(num_threads);

        long long total_sum = 0;
        
        // Inicio de medición de tiempo
        double start_time = omp_get_wtime();

        // Directiva de OpenMP para paralelizar el bucle y manejar la condición de carrera con reduction
        #pragma omp parallel for reduction(+:total_sum)
        for (long long i = 0; i < ARRAY_SIZE; i++) {
            total_sum += arr[i];
        }

        // Fin de medición de tiempo
        double end_time = omp_get_wtime();
        double time_taken = end_time - start_time;

        printf("%d\t%f\t\t%lld\n", num_threads, time_taken, total_sum);
    }

    free(arr);
    return 0;
}
