# EDA — Práctica 1 — Estructuras Unidimensionales

Pilas y Colas en Sistemas Reales: Undo/Redo de un Editor de Código y Control de Tráfico en un Firewall — Implementación desde Cero y Análisis Formal de Complejidad.

---

## Problema 2 — Firewall (Cola circular + ventana deslizante)

### Estructuras implementadas

- **`BuferPaquetes`** (`src/queue_circular.hpp/.cpp`): cola circular sobre arreglo, capacidad fija `C`. Búfer de recepción de paquetes.
- **`ColaTimestamps`** (`src/queue_timestamps.hpp/.cpp`): cola circular sobre arreglo, guarda timestamps para la ventana deslizante del limitador de tasa.
- **`ColaTimestampsLista`** (`src/queue_list.hpp/.cpp`): misma interfaz que `ColaTimestamps`, implementada con lista enlazada. Cumple el requisito de segunda representación (sección 7 del enunciado).
- **`ratelimiter`** (`src/ratelimiter.hpp/.cpp`): conecta las colas y decide, por cada paquete, si se ACEPTA, se rechaza por BÚFER LLENO, o se rechaza por LÍMITE DE TASA.

### Decisión de diseño — borde de la ventana `t = t0 + T`

Se definió que el borde **queda incluido dentro de la ventana** (no se purga en ese instante exacto). Se implementa en `purgarExpirados` con la condición estricta `valorFrente < limite` (no `<=`). Ver Caso de prueba 5 para la evidencia.

### Configuración de parámetros (C, T, L)

Los parámetros `C` (capacidad búfer), `T` (duración ventana en ms) y `L` (límite de paquetes por ventana) se manejan como argumentos de línea de comandos (no están fijos en el código), o alternativamente vía archivo de configuración en texto plano simple (sin JSON, para evitar dependencias externas):

```
C=100
T=5000
L=10
```

---

## Generador de datos sintéticos

`data/generador_paquetes.cpp` genera paquetes sintéticos con formato `timestamp_ms, tamaño_bytes`, con semilla fija para reproducibilidad (semilla por defecto: `42`).

**Compilar:**
```
g++ -std=c++17 -O2 -o generador data/generador_paquetes.cpp
```

**Uso:** `.\generador.exe <cantidad> <semilla>`

**Generar los 4 tamaños necesarios para la sección 9 (experimentación):**
```
.\generador.exe 1000 42
.\generador.exe 10000 42
.\generador.exe 100000 42
.\generador.exe 1000000 42
```

Cada corrida crea un archivo separado: `data/paquetes_<n>.txt`.

---

## Programa de experimentación (medición de tiempos)

`src/experimento_firewall.cpp` mide el tiempo de procesamiento con `std::chrono::high_resolution_clock` y reporta estadísticas de aceptación/rechazo.

**Compilar:**
```
g++ -std=c++17 -O2 -o experimento src/experimento_firewall.cpp src/queue_circular.cpp src/queue_timestamps.cpp src/ratelimiter.cpp
```

**Uso:** `.\experimento.exe <archivo> <C> <T> <L>`

**Correr los 4 tamaños** (repetir cada uno 5 veces según la sección 9, registrando media y desviación estándar del tiempo):
```
.\experimento.exe data/paquetes_1000.txt 100 1000 50
.\experimento.exe data/paquetes_10000.txt 100 1000 50
.\experimento.exe data/paquetes_100000.txt 100 1000 50
.\experimento.exe data/paquetes_1000000.txt 100 1000 50
```

---

## Programa de pruebas manuales (casos límite)

`src/test_manual_ratelimiter.cpp` corre un archivo de prueba puntual contra el `ratelimiter`, sin pasar por el `main.cpp` general — útil para validar casos límite ajustando `C`, `capacidadVentana`, `T`, `L` y `archivoPrueba` directamente en el código.

**Compilar (sin el main general del proyecto):**
```
g++ -std=c++17 -o test_manual src/test_manual_ratelimiter.cpp src/queue_circular.cpp src/queue_timestamps.cpp src/ratelimiter.cpp
```

**Ejecutar:**
```
.\test_manual.exe
```

---

## Casos de prueba obligatorios — Problema 2 (sección 11 del enunciado)

| # | Caso | Archivo de entrada | Parámetros (C, T, L) | Resultado esperado | Resultado obtenido |
|---|---|---|---|---|---|
| 1 | Flujo normal por debajo de C y L | `tests/p2_caso1_flujo_normal.txt` | C=100, T=10, L=5 | Todos los paquetes aceptados | Aceptados: __ |
| 2 | Consulta sobre búfer vacío | (archivo vacío) | C=100, T=10, L=1 | 0 en todos los contadores | Procesados: 0, Aceptados: 0 |
| 3 | Búfer exactamente lleno + paquete adicional | `tests/p2_caso3_bufer_lleno.txt` | C=2, T=..., L=... | El paquete extra se rechaza por búfer lleno | Rechazados por búfer: __ |
| 4 | Ráfaga que excede L dentro de T | `tests/p2_caso4_rafaga_excede_L.txt` | C=100, T=..., L=... | Los paquetes que exceden L se rechazan por tasa | Rechazados por tasa: __ |
| 5 | Paquete exactamente en el borde t = t0 + T | `tests/p2_caso5_borde_ventana.txt` | C=100, T=10, L=1 | El segundo paquete se rechaza por tasa (el borde cuenta dentro de la ventana) | Aceptados: 1, Rechazados por tasa: 1 |
| 6 | Dequeue sobre búfer vacío | — (prueba directa, sin archivo) | — | `dequeue()` retorna `false` sin crashear | "dequeue() retorno false: el bufer estaba vacio, como se esperaba." |
| 7 | Un único paquete | `tests/p2_caso7_un_paquete.txt` | C=100, T=10, L=5 | El paquete se acepta | Aceptados: 1 |

---

## Incidencias durante el desarrollo (evidencia de proceso)

Durante la implementación se detectó un **bug crítico de desbordamiento de memoria** en `BuferPaquetes::enqueue()`: la línea incrementaba por error la variable `capacidad` en lugar de `cantidad`, causando que `tail` terminara apuntando fuera del arreglo real tras cientos de paquetes procesados (crash silencioso, sin excepción). Se diagnosticó aislando `BuferPaquetes` y `ColaTimestamps` por separado (comentando su uso uno a la vez) hasta confirmar la causa raíz, y se corrigió cambiando `capacidad++` por `cantidad++`. Verificado con archivos de hasta 1,000,000 de paquetes sin fallos posteriores.

---

## Notas de compilación general

- Todos los `.exe`, `.o` y archivos de salida temporal (`salida.txt`) están excluidos vía `.gitignore` — no se versionan, se regeneran compilando el código fuente.
- Compilación completa del proyecto (una vez integrado con `main.cpp`):
```
g++ -std=c++17 -O2 -o ada_p1 src/*.cpp
```