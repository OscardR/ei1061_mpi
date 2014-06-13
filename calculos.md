# Cálculos teóricos

### Tiempo de difusión de un mensaje de longitud _n_

![T(n)=alpha+beta·n](img/t.png)

## Broadcast de MPI

![Difusión por MPI Broadcast](img/broadcast.png)

Tiempo de difusión teórico usando el broadcast de MPI:

![TB(n)=log_2{p}·(alpha+beta·n)](img/tb.png)

## Difusión por broadcast lineal

Cada proceso ha de hacer un broadcast. Por lo tanto el tiempo total de la difusión es:

![T_{BL}=p·log_2{p}·(alpha+beta·n)](img/tbl.png)