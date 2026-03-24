# Ejercicio 4: Secuenciacion de Tareas con Plazos

## 1. Descripcion del problema

Se tiene una sola maquina y un conjunto de `n` tareas. Cada tarea `i` tiene:

- una ganancia `g[i]` si termina antes o en su plazo,
- un plazo `d[i]` (tiempo limite),
- duracion fija de `1` unidad de tiempo.

Se debe seleccionar un subconjunto de tareas y asignar cada una a un instante entero `t` tal que:

- `1 <= t <= d[i]` para cada tarea elegida,
- no haya dos tareas en el mismo instante.

Objetivo: maximizar la suma total de ganancias.

---

## 2. Criterio greedy

La estrategia greedy clasica para este problema es:

1. Ordenar tareas por ganancia descendente (primero las mas rentables).
2. Recorrer tareas en ese orden.
3. Para cada tarea, intentar ubicarla en el **ultimo instante libre** que no exceda su plazo (`t <= d[i]`).
4. Si existe un instante libre, se asigna; si no, se descarta.

### Estructuras usadas

- `vector<Task>` para almacenar tareas.
- `vector<bool> occupied` para marcar si un instante esta ocupado.
- `vector<int> assignedTimeById` para registrar en que instante quedo cada tarea.
- `vector<ScheduledTask>` para reconstruir y mostrar la solucion final.

### Iteraciones una a una

Para cada tarea del orden greedy:

- Se revisan los instantes desde `min(d[i], Dmax)` hacia `1`.
- Al encontrar el primero libre, se ocupa y se suma `g[i]`.
- Si no hay libre, la tarea no entra en el cronograma.

### Solucion obtenida

La salida incluye:

- ganancia maxima total alcanzada,
- indices de tareas seleccionadas,
- instantes asignados a esas tareas.

---

## 3. Justificacion de optimalidad

La intuicion greedy es priorizar tareas con mayor aporte economico. Asignar cada tarea en el **ultimo** instante posible es clave, porque:

- deja libres instantes tempranos para tareas con plazos mas estrictos,
- evita "bloquear" posiciones que otras tareas no podrian usar.

Argumento de intercambio (idea):

- Sea una solucion optima cualquiera.
- Si en cierta posicion temporal hay una tarea de menor ganancia y existe una tarea no incluida de mayor ganancia que puede colocarse sin violar plazos, intercambiarlas no reduce factibilidad y aumenta (o mantiene) la ganancia.
- Repitiendo estos intercambios, se puede transformar una solucion optima en otra que respeta el orden por ganancia usado por el algoritmo.

Por tanto, la estrategia greedy produce una solucion optima para este problema de tareas unitarias con plazo y ganancia.

---

## 4. Pseudocodigo

### Conjunto de entradas

- `n`: numero de tareas.
- Para cada tarea `i` en `1..n`: `g[i]` (ganancia), `d[i]` (plazo).

### Candidatos

- Tareas aun no procesadas en el arreglo ordenado por ganancia descendente.

### Restricciones

- Cada tarea ocupa exactamente 1 unidad de tiempo.
- Una tarea seleccionada debe ubicarse en `t <= d[i]`.
- Un unico trabajo por instante.

### Funcion objetivo

Maximizar:

`sumatoria de g[i] para todas las tareas seleccionadas`

### Pseudocodigo general

```text
ALGORITMO JobSequencingGreedy
ENTRADA: n, pares (g[i], d[i]) para i=1..n
SALIDA: ganancia_total, tareas_seleccionadas, instantes_asignados

1. Crear lista tareas con campos: id=i, profit=g[i], deadline=d[i]
2. Dmax <- maximo deadline de todas las tareas
3. Ordenar tareas por:
	a) profit descendente
	b) deadline ascendente (desempate)
	c) id ascendente (desempate)

4. occupied[1..Dmax] <- falso
5. assignedTimeById[1..n] <- 0
6. ganancia_total <- 0

7. PARA cada tarea en tareas ordenadas HACER
8.     asignada <- falso
9.     PARA t <- min(tarea.deadline, Dmax) HASTA 1 (decreciendo) HACER
10.        SI occupied[t] = falso ENTONCES
11.            occupied[t] <- verdadero
12.            assignedTimeById[tarea.id] <- t
13.            ganancia_total <- ganancia_total + tarea.profit
14.            asignada <- verdadero
15.            ROMPER
16.        FIN SI
17.    FIN PARA
18. FIN PARA

19. Construir lista seleccionadas con tareas cuyo assignedTimeById[id] > 0
20. Ordenar seleccionadas por instante asignado ascendente
21. Retornar ganancia_total, ids seleccionados e instantes
FIN ALGORITMO
```

---

## 5. Analisis de tiempo y espacio

Sea `n` el numero de tareas y `Dmax <= n`.

- Ordenamiento de tareas: `O(n log n)`.
- Para cada tarea se busca un slot hacia atras hasta `1`: en el peor caso `O(Dmax)`.
- Costo total de asignacion: `O(n * Dmax)`.
- Como `Dmax <= n`, complejidad total: `O(n^2)` en peor caso.

Espacio adicional:

- arreglo de ocupacion `occupied`: `O(Dmax)`.
- arreglos/listas auxiliares de tareas y asignaciones: `O(n)`.

En total, espacio: `O(n)`.

---

## Implementacion POO en C++

La solucion se implementa en `main.cpp` usando clases:

- `Task`: representa cada tarea (`id`, `profit`, `deadline`).
- `ScheduledTask`: representa una tarea ya asignada (`id`, `assignedTime`).
- `JobSequencer`: encapsula lectura, algoritmo greedy y construccion de salida.

## Comandos

g++ -std=c++17 -O2 -Wall -Wextra -pedantic main.cpp -o main.exe

./main.exe
./main.exe < input.txt