#include <stdio.h>
#include <mpi.h>

int main( int argc, char ** argv ) {

	int mi_id, num_procs, dato, i;
	MPI_Status st;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mi_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	printf("Hola desde el proceso %d de %d\n", mi_id, num_procs);

	// Ejercicio 1. Anillo
	//----------------------------------------------------------
	dato = 0;
	for (i = 0; i < num_procs; i++) {
		if (i == mi_id) {
			if (mi_id != 0) { 
				MPI_Recv(&dato, 1, MPI_INT, mi_id - 1, 777, MPI_COMM_WORLD, &st);
				dato++;
			}
			if (mi_id != num_procs - 1) MPI_Send(&dato, 1, MPI_INT, mi_id + 1, 777, MPI_COMM_WORLD);	
		}
	}
	printf("Soy %d/%d, mi dato es %d\n", mi_id, num_procs, dato);
	MPI_Barrier(MPI_COMM_WORLD);
	//-----------------------------------------------------------

	MPI_Finalize();

	return 0;
}