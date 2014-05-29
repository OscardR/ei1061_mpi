#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

/*! Número que identifica a cada proceso. Es asignado automáticamente por MPI al ejecutar con mpirun. */
int mi_id;

/*! Número de procesos. Es establecido automáticamente por MPI al ejecutar con mpirun. */
int num_procs;

/*! Número de etapas de la compartición de elementos.
 * Se calcula como \f$\log_{2} N\f$, siendo N el número de procesos.
 */
int etapas; 

/*! Vector con los valores a repartir entre procesos. */
int * vector;

/*! Número de elementos del vector que posee cada proceso. Este parámetro puede cambiarse especificando otro valor como primer parámetro desde la línea de comandos */
int MULTIPLO = 1;

/*! Muestra el vector del proceso actual por pantalla, junto al identificador del proceso */
void muestra_vector( void ) {
	printf( "Proceso [%d], vector: [", mi_id );
	int N = num_procs * MULTIPLO;
	for ( int i = 0; i < N; i++ ) {
		printf( "%d", vector[i] );
		if ( i < N - 1 ) printf( ", " );
		else printf( "]\n" );
	}
}

/*! @param n número de procesos
 * @return número de etapas. Equivale a \f$\log_{2} n\f$
 */
int calcula_etapas( int n ) {
	int e = 0;
	while ( n >>= 1 ) { ++e; }
	return e;
}

/*! Función main.
 * Inicializa MPI y ejecuta el algoritmo de compartición. 
 *
 * @param argv el primer parámetro del CLI especifica el número de elementos del vector que pertenecen a cada proceso (`MULTIPLO`). Por defecto `MULTIPLO` es 1.
 * @return 0 si todo ha ido bien.
 */
int main( int argc, char ** argv ) {

	MPI_Status st;
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &mi_id );
	MPI_Comm_size( MPI_COMM_WORLD, &num_procs );

	if ( argc > 1 ) MULTIPLO = atoi( argv[1] );

	vector = ( int * ) malloc( MULTIPLO * num_procs * sizeof( int ) );
	etapas = calcula_etapas( num_procs );

	//! Inicializa el vector:
	for ( int i = 0; i < MULTIPLO * num_procs; i++ ) {
		//! - Cada proceso tiene su identificador como valores de su tramo del vector y el resto de valores serán -1.
		if ( i >= mi_id * MULTIPLO && i < (mi_id + 1) * MULTIPLO )
			vector[i] = mi_id; 
		else 
			vector[i] = -1;
	}

	// El proceso 0 imprime las cabeceras para ver los vectores antes de la compartición.
	if( mi_id == 0 ) printf( "\nVectores antes:\n===============\n" );
	MPI_Barrier( MPI_COMM_WORLD );

	//! - Cada proceso imprime su vector.
	muestra_vector();
	MPI_Barrier( MPI_COMM_WORLD );

	// El proceso 0 imprime las cabeceras para mostrar los envíos
	if( mi_id == 0 ) printf( "\nEnvíos:\n===============\n" );
	MPI_Barrier( MPI_COMM_WORLD );

	//! Se inicializan variables para el algoritmo, y comienza el bucle de etapas.
	int size, orig, dest, seg_send, seg_recv;
	int envios = 0;

	for ( int i = 0; i < etapas; i++ ) {

		/*! - Se calcula el tamaño de la ventana de envíos de la etapa 
		 * actual. La fórmula es:
		 *
		 * \f$S=2^e\f$, teniendo que e es el número de etapa actual.
		 */
		size = pow(2, i);

		/*! - Se calcula si el proceso es origen o destino en función 
		 * de su identificador. Un proceso origen primero ENVÍA y 
		 * luego RECIBE. Un proceso destino primero RECIBE y luego 
		 * ENVÍA.
		 *
		 * Se calcula en base a la siguiente fórmula:
		 *
		 * \f$i\mod(S\cdot 2)<S\f$, teniendo que i es el número del 
		 * proceso, y S el tamaño de la ventana de envíos de la etapa 
		 * actual.
		 */

		if ( mi_id % ( size * 2 ) < size ) { // Proceso origen
			// Calculamos quién es el nodo destino
			dest = mi_id + size;
			seg_send = (mi_id - mi_id % size);
			seg_recv = (dest - dest % size);

			/*! - **Si es origen:** Se realiza el envío primero, imprimiendo por pantalla el nodo que envía y el que recibe, y la parte del vector que se envía.*/
			printf( "[proceso %d] etapa: %d | [%d] --> [%d] {%d,%d}\n", mi_id, i, mi_id, dest, seg_send, seg_send + size - 1);
			MPI_Send(
				& vector[MULTIPLO * seg_send], MULTIPLO * size, MPI_INT, 
				dest, 777, MPI_COMM_WORLD );
			envios++;

			MPI_Recv(
				& vector[MULTIPLO * seg_recv], MULTIPLO * size, MPI_INT, 
				dest, 777, MPI_COMM_WORLD, & st );

		} else { // Proceso destino
			orig = mi_id - size;
			seg_recv = (orig - orig % size);
			seg_send = (mi_id - mi_id % size);

			/*! - **Si es destino:** Se realiza la recepción primero, imprimiendo por pantalla el nodo que envía y el que recibe, y la parte del vector que se envía. */
			printf( "[proceso %d] etapa: %d | [%d] <-- [%d] {%d,%d}\n", mi_id, i, orig, mi_id, seg_send, seg_send + size - 1);
			MPI_Recv(
				& vector[MULTIPLO * seg_recv], MULTIPLO * size, MPI_INT, 
				orig, 777, MPI_COMM_WORLD, & st );	

			MPI_Send(
				& vector[MULTIPLO * seg_send], MULTIPLO * size, MPI_INT, 
				orig, 777, MPI_COMM_WORLD );
			envios++;

		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	// El proceso 0 imprime las cabeceras para mostrar los vectores tras la compartición.
	if(mi_id == 0) printf("-------------\nEnvíos (por proceso): %d [Total: %d]\n\nVectores después:\n=================\n", envios, envios * num_procs);
	MPI_Barrier(MPI_COMM_WORLD);

	/*! - Cada proceso muestra su vector, con los elementos ya compartidos. */
	muestra_vector();

	/*! - Finaliza MPI y retorna el valor 0. */
	MPI_Finalize();
	return 0;
}