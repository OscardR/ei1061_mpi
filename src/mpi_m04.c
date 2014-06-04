#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "utils.h"

/*! @file mpi_m04.c
 *  @brief Laboratorio M4: Recogida global en MPI (III) - Concatenación
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

/*! Número que identifica a cada proceso. Es asignado automáticamente por MPI al ejecutar con mpirun. */
int mi_id;

/*! Número de procesos. Es establecido automáticamente por MPI al ejecutar con mpirun. */
int num_procs;

/*! Número de etapas de la compartición de elementos.
 * Se calcula como \f$N\f$, siendo _N_ el número de procesos.
 */
int etapas; 

/*! Vector con los valores a repartir entre procesos. */
int * vector;

/*! Número de elementos del vector que posee cada proceso. Este parámetro puede cambiarse especificando otro valor como primer parámetro desde la línea de comandos */
int MULTIPLO = 1;

/*! Estado de envíos MPI global para ser usado desde métodos de envío y recepción. */
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
 * @param dest El id del proceso destino
 * @param secc_env El segmento del vector
 */
void envio(int dest, int secc_env) {
	MPI_Send(
		&vector[MULTIPLO * secc_env], 
		MULTIPLO, 
		MPI_INT, 
		dest, 
		777,
		MPI_COMM_WORLD
	);
}

/**
 * Realizar una recepción desde un proceso
 * @param orig El id del proceso origen
 * @param secc_env El segmento del vector
 */
void recepcion(int orig, int secc_env) {
	MPI_Recv(
		&vector[MULTIPLO * secc_env],
		MULTIPLO,
		MPI_INT,
		orig,
		777,
		MPI_COMM_WORLD,
		& st 
	);
}

/*! Función main.
 * Inicializa MPI y ejecuta el algoritmo de **concatenación**. 
 *
 * @param argc parámetro automático con el número de argumentos recibidos desde el CLI
 * @param argv `[1]` el primer parámetro del CLI especifica el número de elementos del vector que pertenecen a cada proceso (`MULTIPLO`). Por defecto `MULTIPLO` es 1.
 * @return 0 si todo ha ido bien.
 */
int main( int argc, char ** argv ) {

	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &mi_id );
	MPI_Comm_size( MPI_COMM_WORLD, &num_procs );

	if ( argc > 1 ) MULTIPLO = atoi( argv[1] );
	printf("MULTIPLO: %d\n", MULTIPLO);

	int N = num_procs * MULTIPLO;
	vector = ( int * ) malloc( N * sizeof( int ) );
	etapas = num_procs;

	//! Inicializa el vector:
	for ( int i = 0; i < N; i++ ) {
		vector[i] = 0;
	}
	for ( int i = 0; i < MULTIPLO; i++ ) {
		//! - Cada proceso tiene su identificador como valores de su tramo del vector y el resto de valores serán 0.
		vector[mi_id * MULTIPLO + i] = mi_id;
	}

	// El proceso 0 imprime las cabeceras para ver los vectores antes de la compartición.
	if( mi_id == 0 ) 
		printf( "\nVectores antes:\n===============\n" );
	MPI_Barrier( MPI_COMM_WORLD );
	
	//! Cada proceso imprime su vector.
	muestra_vector( vector, N, mi_id );
	MPI_Barrier( MPI_COMM_WORLD );

	// El proceso 0 imprime las cabeceras para mostrar los envíos
	if( mi_id == 0 ) 
		printf( "\nEnvíos:\n===============\n" );
	MPI_Barrier( MPI_COMM_WORLD );

	//! Se inicializan las variables de control de comunicaciones.
	int orig, dest, secc_env, secc_rec; 
	int envios = 0;	// Contador de envíos
	
	//! Comienza el bucle con tantas etapas como procesos:
	for ( int e = 0; e < etapas; e++ ) {

		/*! - Se decide quién es el proceso desde el cual vamos a recibir datos 
		 * y cuál será el proceso al cual enviaremos datos con la siguiente
		 * fórmula:
		 *
		 * \f$p_{origen} = (i - 1) \mod n\f$
		 *
		 * \f$p_{destino} = (i + 1) \mod n\f$
		 * , siendo _i_ el número del proceso actual, y _n_ el número de procesos.
		 */
		orig = mod((mi_id - 1), num_procs) ;
		dest = (mi_id + 1) % num_procs;

		/*! - Se decide la sección del vector que se va a enviar y la que se
		 * va a recibir, mediante la siguiente fórmula:
		 *
		 * \f$s_{envío} = (i - e) \mod n\f$
		 *
		 * \f$s_{recepción} = (i - e - 1) \mod n\f$
		 * , siendo _i_ el número del proceso actual, _e_ la etapa actual y _n_ 
		 * el número de procesos.
		 */
		secc_env = mod((mi_id - e), num_procs);
		secc_rec = mod((mi_id - e - 1), num_procs);
		
		printf( "[proceso %d] etapa: %d | [%d] --> [%d] {%d,%d}\n", mi_id, e, orig, dest, secc_env, secc_rec);

		/*! - Se decide si el proceso **envía** primero y **recibe** después o
		 * alrevés en función de si el identificador del proceso es par o impar, y 
		 * se realiza el envío.
		 *
		 * Los procesos pares envían primero y los impares reciben primero.
		 */
		if ( mi_id % 2 == 0 ) {
			envio(dest, secc_env);
			recepcion(orig, secc_rec);
		} else {
			recepcion(orig, secc_rec);
			envio(dest, secc_env);
		}
		envios++;
	} 

	// El proceso 0 imprime las cabeceras para mostrar los vectores tras la compartición.
	if(mi_id == 0) printf("-------------\nEnvíos (por proceso): %d [Total: %d]\n\nVectores después:\n=================\n", envios, envios * num_procs);
	MPI_Barrier(MPI_COMM_WORLD);

	/*! - Cada proceso muestra su vector, con los elementos ya compartidos. */
	muestra_vector( vector, N, mi_id );

	/*! - Finaliza MPI y retorna el valor 0. */
	MPI_Finalize();
	return 0;
}