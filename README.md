# EDA — Práctica 1 — Estructuras Unidimensionales

Pilas y Colas en Sistemas Reales: Undo/Redo de un Editor de Código y Control
de Tráfico en un Firewall — Implementación desde Cero y Análisis Formal de
Complejidad.

Integrantes: Sara Castaño Suárez, Simón López Mesa, Antuan Alejandro Navarro Ortiz

Para el diseño de las soluciones (TAD, decisiones de diseño, comparación de
representaciones), el pseudocódigo, el análisis de complejidad y los
resultados experimentales completos, ver el informe técnico en `report/`.

---

## Requisitos

- Compilador con soporte C++17 (g++ recomendado)
- Sin dependencias externas

---

## Estructura del repositorio

```
├── data/               Generador de datos sintéticos de Problema 2 y paquetes generados
├── report/              Informe técnico y resúmenes de experimentación (Problema 1 y 2)
├── results/             Resultados crudos de cada corrida (5 x 4 tamaños)
├── src/                 Código fuente
├── tests/               Casos de prueba (.txt) y datos sintéticos generados
├── .gitignore
├── Pseudocodigo queue
├── Pseudocodigo_pila.txt
├── README.md
```

---

## Ejecución rápida (Problema 1 y Problema 2)

**Opción A — desde VS Code:** abrir `src/main.cpp` y presionar **F5**. Esto
compila y ejecuta usando la tarea configurada en `.vscode/tasks.json`.

**Opción B — desde terminal:**
```
g++ -std=c++17 -O2 src/main.cpp src/undoredo.cpp src/queue_circular.cpp src/queue_timestamps.cpp src/ratelimiter.cpp src/firewall_runner.cpp -I src -o ejecutable.exe
.\ejecutable.exe
```

El programa pregunta el problema (1 o 2), y para Problema 1 pregunta la
representación de pila (StackArray o StackList) antes de listar los archivos
disponibles en `tests/`. Al finalizar imprime el resumen del documento y el
tiempo de ejecución (medido con `std::chrono`).

> **Nota:** si al ejecutar aparece el mensaje "Presione una tecla para
> continuar . . ." sin mostrar el menú, o la consola se queda pensando sin
> avanzar, cerrar la ventana de terminal y volver a ejecutar (F5 o el comando
> de nuevo) hasta que el menú aparezca correctamente.

---

## Problema 1 — Editor de Texto (Pilas LIFO y Gestor de Undo/Redo)

### Archivos

- `src/stack_array.hpp/.cpp` — Pila sobre arreglo dinámico
- `src/stack_list.hpp/.cpp` — Pila sobre lista enlazada
- `src/undoredo.hpp/.cpp` — `UndoRedoManager`, plantilla parametrizada por representación

### Generador de eventos sintéticos

`src/generador_eventos.cpp` genera eventos `EDIT`/`UNDO`/`REDO` sintéticos,
con semilla fija para reproducibilidad (semilla fija: `42`).

La cantidad de eventos a generar está fija dentro del código (no se recibe
como argumento), en esta línea:

```cpp
int cantidad_eventos = 100000;
```

**Para generar cada tamaño:**

1. Abrir `src/generador_eventos.cpp` y editar el valor de `cantidad_eventos`
   al tamaño deseado (1000, 10000, 100000 o 1000000).
2. Guardar el archivo.
3. Compilar:
   ```
   g++ -std=c++17 -O2 src/generador_eventos.cpp -o generador_eventos.exe
   ```
4. Ejecutar:
   ```
   .\generador_eventos.exe
   ```

Repetir estos 4 pasos por cada uno de los 4 tamaños necesarios para la
experimentación (1000, 10000, 100000, 1000000), cambiando el valor de
`cantidad_eventos` y recompilando cada vez.

Cada corrida genera el archivo directamente en `tests/` con el nombre
`eventos_masivos_<n>.txt`, donde ya queda disponible para seleccionarse
desde el menú del ejecutable principal.

### Casos de prueba y resultados

Los 7 casos obligatorios (Sección 11 del enunciado), corridos sobre ambas
representaciones, y los resultados de experimentación de los 4 tamaños, están
documentados con resultado esperado/obtenido en el informe técnico
(Secciones 8.1 y 9.1). Los archivos de prueba están en `tests/`
(`p1_caso1_...` a `p1_caso7_...`), el resumen de experimentación en
`report/RESULTADOS P1 EXPERIMENTACIÓN`.

---

## Problema 2 — Firewall (Cola circular + ventana deslizante)

### Archivos

- `src/queue_circular.hpp/.cpp` — `BuferPaquetes`, cola circular sobre arreglo
- `src/queue_timestamps.hpp/.cpp` — `ColaTimestamps`, cola circular sobre arreglo
- `src/queue_list.hpp/.cpp` — `ColaTimestampsLista`, cola sobre lista enlazada
- `src/ratelimiter.hpp/.cpp` — conecta las colas, decide ACEPTADO / RECHAZADO_BUFER_LLENO / RECHAZADO_LIMITE_TASA

### Configuración de parámetros (C, T, L)

Se manejan como argumentos de línea de comandos (no están fijos en el
código), o alternativamente vía archivo de configuración en texto plano:

```
C=100
T=5000
L=10
```

### Generador de datos sintéticos

`data/generador_paquetes.cpp` genera paquetes sintéticos con formato
`timestamp_ms, tamaño_bytes`, semilla fija (por defecto `42`).

**Compilar:**

```
g++ -std=c++17 -O2 -o generador data/generador_paquetes.cpp
```

**Uso:** `.\generador.exe <cantidad> <semilla>`

**Generar los 4 tamaños:**

```
.\generador.exe 1000 42
.\generador.exe 10000 42
.\generador.exe 100000 42
.\generador.exe 1000000 42
```

Cada corrida crea `data/paquetes_<n>.txt`.

### Programa de experimentación (medición de tiempos)

`src/experimento_firewall.cpp` mide el tiempo con
`std::chrono::high_resolution_clock` y reporta estadísticas de
aceptación/rechazo.

**Compilar:**

```
g++ -std=c++17 -O2 -o experimento src/experimento_firewall.cpp src/queue_circular.cpp src/queue_timestamps.cpp src/ratelimiter.cpp
```

**Uso:** `.\experimento.exe <archivo> <C> <T> <L>`

**Correr los 4 tamaños** (parámetros usados en la experimentación de la Sección 9.2):

```
.\experimento.exe data/paquetes_1000.txt 777777 66666 3333
.\experimento.exe data/paquetes_10000.txt 777777 66666 3333
.\experimento.exe data/paquetes_100000.txt 777777 66666 3333
.\experimento.exe data/paquetes_1000000.txt 777777 66666 3333
```

### Programa de pruebas manuales (casos límite)

`src/test_manual_ratelimiter.cpp` corre un archivo de prueba puntual contra
el `ratelimiter`, sin pasar por `main.cpp` — útil para validar casos límite
ajustando `C`, `capacidadVentana`, `T`, `L` y `archivoPrueba` directamente en
el código.

**Compilar:**

```
g++ -std=c++17 -o test_manual src/test_manual_ratelimiter.cpp src/queue_circular.cpp src/queue_timestamps.cpp src/ratelimiter.cpp
```

**Ejecutar:** `.\test_manual.exe`

### Casos de prueba y resultados

Los 7 casos obligatorios y los resultados de experimentación de los 4
tamaños están documentados con resultado esperado/obtenido en el informe
técnico (Secciones 8.2 y 9.2). Los archivos de prueba están en `tests/`
(`p2_caso1_...` a `p2_caso7_...`), el resumen de experimentación en
`report/REPORTE P2 EXPERIMENTAL`, y las 20 corridas crudas (4 tamaños x 5
repeticiones) en `results/`.

---

## Incidencias durante el desarrollo

Durante la implementación se detectó un bug crítico de desbordamiento de
memoria en `BuferPaquetes::enqueue()`: la línea incrementaba por error la
variable `capacidad` en lugar de `cantidad`, causando que `tail` terminara
apuntando fuera del arreglo real tras cientos de paquetes procesados (crash
silencioso, sin excepción). Se diagnosticó aislando `BuferPaquetes` y
`ColaTimestamps` por separado hasta confirmar la causa raíz, y se corrigió
cambiando `capacidad++` por `cantidad++`. Verificado con archivos de hasta
1,000,000 de paquetes sin fallos posteriores.

---

## Notas de compilación general

- Todos los `.exe`, `.o` y archivos de salida temporal están excluidos vía
  `.gitignore` — se regeneran compilando el código fuente.
- No se usa `src/*.cpp` en la compilación completa porque `stack_array.cpp`
  y `stack_list.cpp` son implementaciones de plantillas (`template<typename T>`)
  incluidas directamente por su `.hpp`; compilarlas también como unidades de
  traducción aparte produce redefiniciones. Por la misma razón,
  `experimento_firewall.cpp`, `test_manual_ratelimiter.cpp`,
  `generador_eventos.cpp` y `data/generador_paquetes.cpp` (cada uno con su
  propio `main`) se compilan por separado como herramientas independientes.
