# Damas Internacionales (C++)


## Integrantes
- Ana Sofia Llorente Espinosa
- Valentina Orjuela
  
---
Proyecto de consola en C++ modularizado (main.cpp, .h y .cpp) que implementa:
- Tablero 10x10 con casillas diferenciadas.
- 20 piezas por jugador (X para Jugador 1, O para Jugador 2).
- Menú: iniciar partida (2 jugadores), ver reglas, salir.
- Turno indicado, capturas obligatorias y capturas múltiples.
- Coronación (dama) al llegar a la última fila.
- Validación y mensajes de movimientos inválidos.
- Contador de movimientos,sugerencia de movimientos, historial de movimientos y marcador de puntajes.

## Compilación

Con `g++` (C++17):

```
g++ -std=c++17 main.cpp Board.cpp Game.cpp Piece.cpp Menu.cpp -o damas
```

## Ejecución

En Windows (PowerShell o CMD):

```
./damas.exe
```

## Controles en partida

- Ingrese movimientos.
- Si hay captura disponible, la captura es obligatoria.
- Si tras capturar hay otra captura posible con la misma pieza, debe continuar (captura múltiple).
- Escriba `q` para terminar la partida y `v` para sugerencia de movimiento.

## Notas

- Las damas (X/O) se mueven de manera diagonal por el tablero hacia la izquierda y la derecha respectivamente. La ficha solo podra retroceder una vez este coronada (Dama).
- Se suma 1 punto por cada pieza capturada.

