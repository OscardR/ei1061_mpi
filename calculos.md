# Cálculos teóricos

### Tiempo de difusión de un mensaje de longitud _n_

![T(n)=alpha+beta·n](img/t.png)

## Broadcast de MPI

Un proceso envía un dato al resto de procesos. En la imagen, el proceso 0 hace de raíz, y el esquema muestra como el dato del proceso 0 se propaga a todos los procesos de forma logarítmica.

![Difusión por MPI Broadcast](img/broadcast.png)

Tiempo de difusión teórico usando el broadcast de MPI:

![](img/tb.png)
<!---
T_B(n)=\log_2{p}\cdot(\alpha+\beta\cdot n)
-->

## Difusión por broadcast lineal

Cada proceso ha de hacer un broadcast. Por lo tanto el tiempo total de la difusión es:

![](img/tbl.png)
<!---
T_{BL}(n)=p\cdot \log_2{p}\cdot(\alpha+\beta\cdot n)
-->

## Difusión por doblado recursivo

En cada etapa hay _p_ envíos de tamaño exponencialmente proporcional al número de etapa, y el número de etapas es logarítmico.

![Difusión por doblado recursivo](img/recursivo.png)

Tiempo de difusión teórico usando el algoritmo de doblado recursivo:

![](img/tdr.png)
<!---
T_{DR}(n)=\sum_{e=0}^{\log_2{p}}2\cdot(\alpha+\beta\cdot 2^e\cdot n)

        log {p}             
          _2                   e
T  (n) = \    2 (alpha + beta 2 n)
 DR      /__
        e = 0
-->

## Difusión por concatenación

En cada etapa hay _p_ envíos de tamaño base, y el número de etapas es lineal.

![Difusión por concatenación](img/concatenacion.png)

Tiempo de difusión teórico usando el algoritmo de concatenación:

![](img/tc.png)

<!---
T_C(n)=(\alpha+\beta\cdot n)\cdot(p-1)
-->

## Gráficas teóricas comparativas de tiempos de difusión

### 4 procesos

![](img/teorico_4.png)

### 8 procesos

![](img/teorico_8.png)

### 16 procesos

![](img/teorico_16.png)