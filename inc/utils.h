#include <stdio.h>
#include <stdlib.h>

/*! Función para mostrar vectores.
 * @param vector Un array de `int`s para mostrar
 * @param N El tamaño del vector
 * @param id El identificador del proceso que imprime el vector
 */
void muestra_vector( int * vector, int N, int id ) {
	printf( "Proceso [%d], vector[%d]: [", id, N );
	for ( int i = 0; i < N; i++ ) {
		printf( "%d", vector[i] );
		if ( i < N - 1 ) printf( ", " );
		else printf( "]\n" );
	}
}