#include <stdio.h>
#include <mpi.h>

/*!@file mpi_m01.c
 * @brief Laboratorio M1: Introducción a MPI - Comunicación en Anillo
 *
 * # Introducción a MPI
 *
 * - Compilar y ejecutar el programa de ejemplo de MPI en 4 nodos de patan.
 * - Desarrolla, compila y ejecuta un simple test de ping-pong MPI en 2 nodos de patan.
 *
 * ## Comunicación en anillo. 
 */

/*! Función main.
 * Inicializa MPI y ejecuta el algoritmo de envío en anillo. 
 *
 * @param argc parámetro automático con el número de argumentos recibidos desde el CLI
 * @param argv [1] el primer parámetro del CLI especifica el número de elementos del vector que pertenecen a cada proceso (`MULTIPLO`). Por defecto `MULTIPLO` es 1.
 * @return 0 si todo ha ido bien.
 */
 int main( int argc, char ** argv ) {

    //! - Inicializa las variables necesarias
    int mi_id, num_procs, dato, i;

    //! - Inicializa MPI
    MPI_Status st;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &mi_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    //! - Imprime el identificador de cada proceso
    printf("Hola desde el proceso %d de %d\n", mi_id, num_procs);

    //! - Inicializa `dato` a 0
    dato = 0;
    for (i = 0; i < num_procs; i++) {
        if (i == mi_id) {
            if (mi_id != 0) { 
                MPI_Recv(&dato, 1, MPI_INT, mi_id - 1, 777, MPI_COMM_WORLD, & st);
                //! - Cada vez que envía el dato, se incrementa en 1 su valor
                dato++;
            }
            if (mi_id != num_procs - 1) {
                MPI_Send(&dato, 1, MPI_INT, mi_id + 1, 777, MPI_COMM_WORLD);
            }
        }
    }

    //! - Imprime el identificador de cada proceso, junto con su valor para `dato`
    printf("Soy %d/%d, mi dato es %d\n", mi_id, num_procs, dato);
    MPI_Barrier(MPI_COMM_WORLD);

    //! - Finaliza MPI y devuelve 0.
    MPI_Finalize();

    return 0;
}