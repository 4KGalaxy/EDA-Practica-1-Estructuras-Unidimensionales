# EDA---Pr-ctica-1-Estructuras-Unidimensionales
Pilas y Colas en Sistemas Reales: Undo/Redo de un Editor de Código y Control de Tráfico en un Firewall — Implementación desde Cero y Análisis Formal de Complejidad.

usar un archivo de configuración en texto plano simple, no JSON. Por ejemplo config.txt:
C=100
T=5000
L=10

codigo para generar, los datos sinteticos, el primer valor es la cantidad de datos que se requiera, el segundo valor es la semilla que se quiera utilizar, la semilla predefinida es 42.

.\generador.exe 1000 99

Para experimentos de la sección 9, se necesita correr esto 4 veces distintas, cambiando solo n:

.\generador.exe 1000 42
.\generador.exe 10000 42
.\generador.exe 100000 42
.\generador.exe 1000000 42

Codigo de compilado sin utilizar el main general:
g++ -std=c++17 -o test_manual src/test_manual_ratelimiter.cpp src/queue_circular.cpp src/queue_timestamps.cpp src/ratelimiter.cpp

Codigo ejecutable para el test manual:
.\test_manual.exe

Caso prueba 5-limite — Paquete en el borde t = t0 + T
Entrada: paquetes en t=0 y t=10, con T=10, L=1.
Resultado esperado y obtenido: el segundo paquete es rechazado por límite de tasa, porque el timestamp t=0 no se purga al ser evaluado exactamente en t = 0 + T. Esto confirma que la implementación excluye el borde de la purga (lo mantiene dentro de la ventana), consistente con la condición valorFrente < límite (estricta) definida en purgarExpirados.

## Casos de prueba — Problema 2 (Firewall / Cola)

Todas las pruebas se corrieron con `test_manual_ratelimiter.cpp`, ajustando manualmente
las variables `C`, `capacidadVentana`, `T`, `L` y `archivoPrueba` al inicio del `main()`,
recompilando con:

g++ -std=c++17 -o test_manual src/test_manual_ratelimiter.cpp src/queue_circular.cpp src/queue_timestamps.cpp src/ratelimiter.cpp


y ejecutando con `.\test_manual.exe`.

| # | Caso (sección 11) | Archivo de entrada | Parámetros (C, T, L) | Resultado esperado | Resultado obtenido |
|---|---|---|---|---|---|
| 1 | Flujo normal por debajo de C y L | `tests/p2_caso1_flujo_normal.txt` | C=100, T=10, L=5 | Todos los paquetes aceptados | Aceptados: __ |
| 2 | Consulta sobre búfer vacío | (archivo vacío) | C=100, T=10, L=1 | 0 en todos los contadores | Procesados: 0, Aceptados: 0 |
| 3 | Búfer exactamente lleno + paquete adicional | `tests/p2_caso3_bufer_lleno.txt` | C=2, T=..., L=... | El paquete extra se rechaza por búfer lleno | Rechazados por búfer: __ |
| 4 | Ráfaga que excede L dentro de T | `tests/p2_caso4_rafaga_excede_L.txt` | C=100, T=..., L=... | Los paquetes que exceden L se rechazan por tasa | Rechazados por tasa: __ |
| 5 | Paquete exactamente en el borde t = t0 + T | `tests/p2_caso5_borde_ventana.txt` | C=100, T=10, L=1 | El segundo paquete se rechaza por tasa (el borde cuenta dentro de la ventana) | Aceptados: 1, Rechazados por tasa: 1 |
| 6 | Dequeue sobre búfer vacío | — (prueba directa, sin archivo) | — | `dequeue()` retorna `false` sin crashear | "dequeue() retorno false: el bufer estaba vacio, como se esperaba." |
| 7 | Un único paquete | `tests/p2_caso7_un_paquete.txt` | C=100, T=10, L=5 | El paquete se acepta | Aceptados: 1 |

### Decisión de diseño — borde de la ventana deslizante

Se definió que el borde `t = t0 + T` **queda incluido dentro de la ventana** (no se purga
en ese instante exacto). Esto se implementa en `purgarExpirados` con la condición estricta
`valorFrente < limite` (en vez de `<=`). El caso 5 demuestra este comportamiento: un timestamp
en `t=0` sigue contando como vigente cuando se evalúa un nuevo paquete en `t=10` con `T=10`.


experimento firewall:
g++ -std=c++17 -O2 -o experimento src/experimento_firewall.cpp src/queue_circular.cpp src/queue_timestamps.cpp src/ratelimiter.cpp
.\experimento.exe data/paquetes_1000.txt 100 1000 50
.\experimento.exe data/paquetes_10000.txt 100 1000 50
.\experimento.exe data/paquetes_100000.txt 100 1000 50
.\experimento.exe data/paquetes_1000000.txt 100 1000 50