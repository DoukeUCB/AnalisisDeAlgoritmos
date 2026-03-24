## Estructura de Datos Utilizada

Se eligió un `deque` (cola de doble extremo) porque su naturaleza se ajusta bien al problema y ofrece una ventaja práctica sobre una cola simple. El orden en que los procesos ingresan a la CPU no depende de ningún tipo de prioridad especial: los procesos ingresan en el orden en el que llegan y, si no se completan en el tiempo límite, son devueltos al final.

El comportamiento del planificador es básicamente FIFO (First In, First Out), por lo que no se necesita ninguna estructura más compleja. Sin embargo, a diferencia de `std::queue`, el `std::deque` expone iteradores, lo que permite recorrer la colección sin necesidad de copiarla. Esto es relevante para el comando `ESTADO`, que antes requería pasar la cola por valor (generando una copia completa de $p$ elementos), y ahora simplemente recibe una referencia constante (`const deque<Process>&`), eliminando esa asignación extra.

---

## Enfoque de la Solución

Para resolver el problema, se creó una estructura `Proceso` que almacena el `id` y el `tiempo_restante` de ejecución. La fila es entonces un `deque` de procesos.

Se lee la entrada buscando los comandos `NUEVO`, `EJECUTAR` y `ESTADO`:
- **NUEVO**: se leen el `id` y el tiempo para agregar el proceso al final del deque (`push_back`).
- **EJECUTAR**: se verifica que el deque no esté vacío, se extrae el primer proceso (`front` + `pop_front`) y se le resta el tiempo de ejecución. Si terminó se descarta y se imprime; si no, se devuelve al final (`push_back`).
- **ESTADO**: se recorre el deque mediante un iterador de rango (`for` basado en rango sobre la referencia constante) y se imprime el tiempo restante de cada proceso, sin generar ninguna copia.

---

## Pseudocódigo y Análisis de Complejidad

```
Proceso:
    String id
    Int tiempo_restante

Deque<Proceso> cola_procesos

Funcion procesar_comando(entrada):
    Si entrada es "NUEVO id tiempo":
        New Proceso p(id, tiempo)
        cola_procesos.push_back(p)              // O(1) amortizado

    Si entrada es "EJECUTAR quantum":
        Si cola_procesos esta vacia:
            return

        Proceso p = cola_procesos.frente()
        cola_procesos.pop_front()               // O(1)

        tiempo_uso = min(quantum, p.tiempo_restante)
        p.tiempo_restante = p.tiempo_restante - tiempo_uso

        Si p.tiempo_restante > 0:
            Imprimir "Ejecutando " + p.id + " (quedan " + p.tiempo_restante + "ms)"
            cola_procesos.push_back(p)          // O(1) amortizado
        Sino:
            Imprimir "Ejecutando " + p.id + " (COMPLETADO)"

    Si entrada es "ESTADO":
        Para cada proceso p en cola_procesos:   // O(p), sin copia
            Imprimir p.id + "(" + p.tiempo_restante + "ms)"
```

### Complejidad en Tiempo

| Operación   | Complejidad | Detalle |
|-------------|-------------|---------|
| `NUEVO`     | $O(1)$      | `push_back` es constante amortizado |
| `EJECUTAR`  | $O(1)$      | `front`, `pop_front` y `push_back` son $O(1)$ |
| `ESTADO`    | $O(p)$      | Iteración directa sobre referencia, sin copia |

**Total** para $n$ comandos con hasta $p$ procesos activos:

$$T(n) = O(n \cdot p)$$

### Complejidad en Espacio

| Estructura                    | Complejidad | Detalle |
|-------------------------------|-------------|---------|
| `cola_procesos` (deque)       | $O(p)$      | Almacena hasta $p$ procesos activos |
| Auxiliar en `ESTADO`          | $O(1)$      | Solo una referencia constante, sin copia |
| Variables escalares           | $O(1)$      | `command`, `id`, `quantum`, etc. |

$$S(n) = O(p)$$

Con respecto a la versión anterior (usando `std::queue` con paso por valor), se eliminó la asignación auxiliar de $O(p)$ elementos en cada llamada a `ESTADO`, reduciendo la presión sobre el heap y mejorando la localidad de caché.
