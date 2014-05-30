#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "utils.h"

/*! @file mpi_m04.c
 *  @brief Laboratorio M4
 *
 * # Recogida global en MPI (III)
 *
 * Desarrolla una implementación eficiente de _recogida global_ en MPI que utilice 
 * un esquema de **concatenación**. El procedimiento se desarrolla en _p_ etapas. En cada 
 * etapa un proceso envía una parte del vector a su vecino de la derecha y recibe 
 * otra parte de su vecino de la izquierda. En la primera etapa la parte enviada 
 * es la propia. En cada una de las siguientes _p − 1_ etapas, la parte enviada es
 * la recibida en la etapa anterior.
 */

int mi_id, num_procs, dato, i, j, N, etapas;
int * vector;
int MULTIPLO = 1;
MPI_Status st;

/*! Implementación del operador `%` teniendo en cuenta módulos negativos 
 * 
 * @param D Dividendo
 * @param d Divisor
 * @return \f$D \mod d\f$, teniendo en cuenta el signo de _D_. Por ejemplo, `mod(-1, 8)` devolverá `7`
 */
int mod( int D, int d ) {
	if ( D < 0 ) return (d + D) % d;
	return D % d;
}

/**
 * Realizar un envío a un proceso
 * @param dest {int} el id del proceso destino
 * @param ini {int} el segmento del vector
 */
void envio(int dest, int ini) {
	MPI_Send(
		&vector[MULTIPLO * ini], 
		MULTIPLO, 
		MPI_INT, 
		dest, 
		777,
		MPI_COMM_WORLD
	);
}

/**
 * Realizar una recepción desde un proceso
 * @param orig {int} el id del proceso origen
 * @param ini {int} el segmento del vector
 */
void recepcion(int orig, int ini) {
	MPI_Recv(
		&vector[MULTIPLO * ini],
		MULTIPLO,
		MPI_INT,
		orig,
		777,
		MPI_COMM_WORLD,
		& st 
	);
}

int main( int argc, char ** argv ) {

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &mi_id );
	MPI_Comm_size( MPI_COMM_WORLD, &num_procs );

	if ( argc > 1 ) MULTIPLO = atoi( argv[1] );
	printf("MULTIPLO: %d\n", MULTIPLO);

	N = num_procs * MULTIPLO;
	vector = ( int * ) malloc( N * sizeof( int ) );
	etapas = num_procs;

	// Init vector
	for ( i = 0; i < N; i++ ) {
		vector[i] = 0;
	}
	for ( i = 0; i < MULTIPLO; i++ ) {
		vector[mi_id * MULTIPLO + i] = mi_id;
	}

	if( mi_id == 0 ) 
		printf( "\nVectores antes:\n===============\n" );
	MPI_Barrier( MPI_COMM_WORLD );
	muestra_vector( vector, N, mi_id );
	MPI_Barrier( MPI_COMM_WORLD );

	if( mi_id == 0 ) 
		printf( "\nEnvíos:\n===============\n" );
	MPI_Barrier( MPI_COMM_WORLD );

	int orig, dest, ini, fin; //! Variables de control de comunicaciones
	int envios = 0;	//! Contador de envíos
	
	for ( i = 0; i < etapas; i++ ) {

		orig = mod((mi_id - 1), num_procs) ;
		dest = (mi_id + 1) % num_procs;

		ini = mod((mi_id - i), num_procs);
		fin = mod((mi_id - i - 1), num_procs);
		
		printf( "[proceso %d] etapa: %d | [%d] --> [%d] {%d,%d}\n", mi_id, i, orig, dest, ini, fin);

		if ( mi_id % 2 == 0 ) {
			envio(dest, ini);
			recepcion(orig, fin);
		} else {
			recepcion(orig, fin);
			envio(dest, ini);
		}
		envios++;
	} 


	if(mi_id == 0) printf("-------------\nEnvíos (por proceso): %d [Total: %d]\n\nVectores después:\n=================\n", envios, envios * num_procs);
	MPI_Barrier(MPI_COMM_WORLD);
	muestra_vector( vector, N, mi_id );

	MPI_Finalize();
	return 0;
}