#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MULTIPLO 4

int mi_id, num_procs, dato, i, N;
int * vector;

void muestra_vector(void) {
	printf("Proceso [%d], vector: [", mi_id);
	for (i = 0; i < N; i++) {
		printf("%d", vector[i]);
		if (i < N - 1) printf(", ");
		else printf("]\n");
	}
}

/*! Función main.
 * Inicializa MPI y ejecuta el algoritmo de compartición. 
 *
 * @param argc parámetro automático con el número de argumentos recibidos desde el CLI
 * @param argv [1] el primer parámetro del CLI especifica el número de elementos del vector que pertenecen a cada proceso (`MULTIPLO`). Por defecto `MULTIPLO` es 1.
 * @return 0 si todo ha ido bien.
 */
int main( int argc, char ** argv ) {

	// MPI_Status st; // No necesitamos status en broadcast
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mi_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	N = num_procs * MULTIPLO;
	vector = (int *)malloc(N * sizeof(int));

	// Init vector
	for (i = 0; i < MULTIPLO; i++) {
		vector[mi_id * MULTIPLO + i] = mi_id;
	}

	muestra_vector();

	for (i = 0; i < num_procs; i++) {
		MPI_Bcast(&vector[MULTIPLO * i], MULTIPLO, MPI_INT, i, MPI_COMM_WORLD);
	}

	muestra_vector();

	MPI_Finalize();
	return 0;
}