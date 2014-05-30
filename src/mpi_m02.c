#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/*! @file mpi_m02.c
 *  @brief Laboratorio M2.1
 *
 * # Recogida global en MPI (I)
 *
 * Desarrolla una implementación de _recogida global_ en MPI en la que 
 * _p_ procesos deben conseguir una copia de todos los elementos de un 
 * vector de _n_ datos inicialmente repartido a partes iguales entre
 * todos ellos (es decir, cada proceso tiene inicialmente _n/p_ elementos 
 * del vector y, para simplificar el problema, se supondrá que _n_ es 
 * múltiplo de _p_). Se trata de la misma operación que implementa la
 * función `MPI_Allgather` de la biblioteca MPI.
 *
 * Dicha difusión debe realizarse utilizando las siguiente estrategias:
 *
 * - **Difusión lineal.** Uno tras otro, cada uno de los procesos envía su 
 * parte del vector a los restantes procesos mediante una operación de 
 * difusión de MPI (MPI Bcast).
 *
 * - **Doblado recursivo.** Este algoritmo funciona en \f$log_2 p\f$ etapas. 
 * En la primera etapa, aquellos procesos que están a una distancia de 1 (en 
 * relación a su identificador) intercambian sus partes
 * del vector. En la segunda etapa, los procesos que están a una 
 * distancia 2 intercambian sus partes del vector (incluidas aquellas 
 * que recibieron en la etapa anterior). En la tercera etapa
 * realizarán el intercambio aquellos procesos que están a una 
 * distancia 4 y así sucesivamente. Para simplificar el problema, se supondrá 
 * que p es una potencia de 2.
 */

//! Define cuantos elementos del vector posee cada proceso.
#define MULTIPLO 4

//! Número que identifica a cada proceso. Es asignado automáticamente por MPI al ejecutar con `mpirun`.
int mi_id;

//! Número de procesos. Es establecido automáticamente por MPI al ejecutar con `mpirun`.
int num_procs;

//! Contador. Se usa en los diferentes bucles del programa.
int i;

/*! Vector con los valores a repartir entre procesos. */
int * vector;

/*! Muestra el vector del proceso actual por pantalla, junto al identificador del proceso */
void muestra_vector(void) {
	printf("Proceso [%d], vector: [", mi_id);
		int N = num_procs * MULTIPLO;
	for (i = 0; i < N; i++) {
		printf("%d", vector[i]);
		if (i < N - 1) printf(", ");
		else printf("]\n");
	}
}

/*! Función main.
 * Inicializa MPI y ejecuta el algoritmo de **difusión lineal**. 
 *
 * @param argc parámetro automático con el número de argumentos recibidos desde el CLI
 * @param argv [1] el primer parámetro del CLI especifica el número de elementos del vector que pertenecen a cada proceso (`MULTIPLO`). Por defecto `MULTIPLO` es 1.
 * @return 0 si todo ha ido bien.
 */
int main( int argc, char ** argv ) {

	//! ### PASOS:
	//! - Se inicializa el sistema MPI.
	// MPI_Status st; // No necesitamos status en broadcast
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &mi_id);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	//! - Se inicializa el vector y se rellena el área de cada proceso con un valor igual al ID del proceso.
	vector = (int *)malloc(num_procs * MULTIPLO * sizeof(int));
	for (i = 0; i < MULTIPLO; i++) {
		vector[mi_id * MULTIPLO + i] = mi_id;
	}

	//! - Cada proceso muestra su vector.
	muestra_vector();

	//! - Cada proceso hace un _broadcast_ al resto de procesos con su trozo del vector.
	for (i = 0; i < num_procs; i++) {
		MPI_Bcast(&vector[MULTIPLO * i], MULTIPLO, MPI_INT, i, MPI_COMM_WORLD);
	}

	//! - Cada proceso vuelve a mostrar su vector.
	muestra_vector();

	//! - Se finaliza el sistema MPI y se retorna 0.
	MPI_Finalize();
	return 0;
}