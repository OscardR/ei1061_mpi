Ejemplo de ejecución
====================

Para ejecutar el código con 8 procesos y 2 elementos del vector para cada proceso (el vector tendría en total 16 enteros), este es el comando:

	oscar@k70ic:~/workspace/c/mpi$ mpirun -np 8 mpi_m02 2

Una posible salida sería la siguiente:

	Vectores antes:
	===============
	Proceso [6], vector: [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 6, 6, -1, -1]
	Proceso [4], vector: [-1, -1, -1, -1, -1, -1, -1, -1, 4, 4, -1, -1, -1, -1, -1, -1]
	Proceso [2], vector: [-1, -1, -1, -1, 2, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
	Proceso [0], vector: [0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
	Proceso [5], vector: [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 5, 5, -1, -1, -1, -1]
	Proceso [1], vector: [-1, -1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1]
	Proceso [7], vector: [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 7, 7]
	Proceso [3], vector: [-1, -1, -1, -1, -1, -1, 3, 3, -1, -1, -1, -1, -1, -1, -1, -1]

	Envíos:
	===============
	[proceso 6] etapa: 0 | [6] --> [7] {6,6}
	[proceso 2] etapa: 0 | [2] --> [3] {2,2}
	[proceso 1] etapa: 0 | [0] <-- [1] {1,1}
	[proceso 4] etapa: 0 | [4] --> [5] {4,4}
	[proceso 5] etapa: 0 | [4] <-- [5] {5,5}
	[proceso 3] etapa: 0 | [2] <-- [3] {3,3}
	[proceso 0] etapa: 0 | [0] --> [1] {0,0}
	[proceso 7] etapa: 0 | [6] <-- [7] {7,7}
	[proceso 6] etapa: 1 | [4] <-- [6] {6,7}
	[proceso 7] etapa: 1 | [5] <-- [7] {6,7}
	[proceso 4] etapa: 1 | [4] --> [6] {4,5}
	[proceso 5] etapa: 1 | [5] --> [7] {4,5}
	[proceso 3] etapa: 1 | [1] <-- [3] {2,3}
	[proceso 2] etapa: 1 | [0] <-- [2] {2,3}
	[proceso 0] etapa: 1 | [0] --> [2] {0,1}
	[proceso 1] etapa: 1 | [1] --> [3] {0,1}
	[proceso 2] etapa: 2 | [2] --> [6] {0,3}
	[proceso 6] etapa: 2 | [2] <-- [6] {4,7}
	[proceso 0] etapa: 2 | [0] --> [4] {0,3}
	[proceso 4] etapa: 2 | [0] <-- [4] {4,7}
	[proceso 5] etapa: 2 | [1] <-- [5] {4,7}
	[proceso 1] etapa: 2 | [1] --> [5] {0,3}
	[proceso 3] etapa: 2 | [3] --> [7] {0,3}
	[proceso 7] etapa: 2 | [3] <-- [7] {4,7}
	-------------
	Envíos (por proceso): 3 [Total: 24]

	Vectores después:
	=================
	Proceso [5], vector: [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7]
	Proceso [3], vector: [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7]
	Proceso [1], vector: [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7]
	Proceso [7], vector: [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7]
	Proceso [6], vector: [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7]
	Proceso [4], vector: [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7]
	Proceso [2], vector: [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7]
	Proceso [0], vector: [0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7]
