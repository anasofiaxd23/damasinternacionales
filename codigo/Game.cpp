#include "Game.h"
#include <iostream>
#include <limits>

Game::Game() : jugadorActual(Player::PLAYER1), conteoMovimientos(0), puntajeJ1(0), puntajeJ2(0), enEjecucion(true) {}

void Game::showMenu() {
    std::cout << "\n // Damas Internacionales //\n";
    std::cout << "1) Iniciar partida\n";
    std::cout << "2) Ver reglas\n";
    std::cout << "0) Salir\n";
    std::cout << "Seleccione una opcion: ";
}

void Game::showRules() const {
    std::cout << "\nReglas basicas:\n";
    std::cout << "- 10x10 casillas; se juega en casillas oscuras.\n";
    std::cout << "- Cada jugador inicia con 20 piezas (x para J1, o para J2).\n";
    std::cout << "- Turno indicado en pantalla; las capturas son OBLIGATORIAS.\n";
    std::cout << "- Se permiten capturas multiples encadenadas.\n";
    std::cout << "- Coronacion: al llegar a la ultima fila se convierte en Dama (X/O).\n";
    std::cout << "- Se cuentan movimientos y puntajes (1 punto por pieza capturada).\n";
    std::cout << "- Entrada de movimientos: (f_c)origen, (f_c) destino en la cual fila represanta f y columnas c, debe ir sin parentesis y el _ debe ser remplazado por un espacio .\n";
    std::cout << "- Durante la partida, escriba q para terminar la partida y volver al menu, y 'v' para sugerencias de movimientos.\n\n";
}

void Game::run() {
    while (enEjecucion) {
        showMenu();
        int opt = -1;
        if (!(std::cin >> opt)) {
            return;
        }
        switch (opt) {
            case 1:
                startTwoPlayers();
                break;
            case 2:
                showRules();
                break;
            case 0:
            default:
                enEjecucion = false;
                break;
        }
    }
}

void Game::startTwoPlayers() {
    tablero = Board();
    tablero.setupInitial();
    jugadorActual = Player::PLAYER1;
    conteoMovimientos = 0;
    puntajeJ1 = 0;
    puntajeJ2 = 0;
    historial.clear();

    while (true) {
        if (!tablero.hasAnyMoves(jugadorActual)) {
            std::cout << "\n" << playerName(jugadorActual) << " no tiene movimientos. Fin de la partida.\n";
            std::cout << "Marcador final: X=" << puntajeJ1 << " | O=" << puntajeJ2 << "\n";
            break;
        }
        bool continueGame = takeTurn();
        if (!continueGame) break; 
        switchTurn();
    }
}

void Game::switchTurn() {
    jugadorActual = (jugadorActual == Player::PLAYER1) ? Player::PLAYER2 : Player::PLAYER1;
}

bool Game::shouldPromote(Player jugador, int fila) const {
    if (jugador == Player::PLAYER1) return fila == 0;
    if (jugador == Player::PLAYER2) return fila == 9;
    return false;
}

bool Game::applyNormalMove(const Position& desde, const Position& hasta, std::string& resumen) {
    auto pieza = tablero.get(desde.fila, desde.columna);
    if (!pieza.has_value()) return false;
   
    auto dests = tablero.getNormalDestinationsFrom(desde.fila, desde.columna);
    bool ok = false;
    for (auto d : dests) if (d.fila == hasta.fila && d.columna == hasta.columna) { ok = true; break; }
    if (!ok) return false;

    
    tablero.set(hasta.fila, hasta.columna, pieza);
    tablero.remove(desde.fila, desde.columna);

   
    auto movida = tablero.get(hasta.fila, hasta.columna);
    if (movida.has_value() && movida->tipo == PieceType::MAN && shouldPromote(movida->propietario, hasta.fila)) {
        tablero.set(hasta.fila, hasta.columna, Piece{PieceType::KING, movida->propietario});
        resumen += " (coronacion)";
    }
    return true;
}

bool Game::applyCaptureMove(const Position& desde, const Position& hasta, std::string& resumen, int& capturas) {
    auto pieza = tablero.get(desde.fila, desde.columna);
    if (!pieza.has_value()) return false;

    auto dests = tablero.getCaptureDestinationsFrom(desde.fila, desde.columna);
    bool ok = false;
    for (auto d : dests) if (d.fila == hasta.fila && d.columna == hasta.columna) { ok = true; break; }
    if (!ok) return false;

   
    int fCaptura = desde.fila + (hasta.fila - desde.fila)/2;
    int cCaptura = desde.columna + (hasta.columna - desde.columna)/2;
    auto capturada = tablero.get(fCaptura, cCaptura);
    if (!capturada.has_value() || capturada->propietario == pieza->propietario) return false;

    
    tablero.remove(fCaptura, cCaptura);
    tablero.set(hasta.fila, hasta.columna, pieza);
    tablero.remove(desde.fila, desde.columna);
    capturas += 1;

   
    if (pieza->propietario == Player::PLAYER1) puntajeJ1 += 1; else puntajeJ2 += 1;

    
    auto movida = tablero.get(hasta.fila, hasta.columna);
    if (movida.has_value() && movida->tipo == PieceType::MAN && shouldPromote(movida->propietario, hasta.fila)) {
        tablero.set(hasta.fila, hasta.columna, Piece{PieceType::KING, movida->propietario});
        resumen += " (coronacion)";
    }
    return true;
}

bool Game::takeTurn() {
    std::cout << "\nTurno: " << playerName(jugadorActual) << "\n";
    tablero.draw();
    std::cout << "Movimientos realizados: " << conteoMovimientos << " | Marcador X=" << puntajeJ1 << " O=" << puntajeJ2 << "\n";

    bool capturaObligatoria = tablero.hasAnyCapture(jugadorActual);
    if (capturaObligatoria) {
        std::cout << "Hay captura obligatoria disponible. Debe capturar.\n";
    }

    std::string resumen = playerName(jugadorActual) + ": ";
    int capturasEncadenadas = 0;

    while (true) {
        std::cout << "Ingrese movimiento (f_C) origen (f_C) destino), 'v' para ver sugerencia, o 'q' para salir: ";
        std::string entrada;
        std::cin >> entrada;
        if (entrada == "q" || entrada == "Q") {
            std::cout << "Partida terminada por el usuario.\n";
            return false;
        }
        if (entrada == "v" || entrada == "V") {
            showSuggestion();
            continue;
        }
        
        int fOrigen = std::stoi(entrada);
        int cOrigen, fDestino, cDestino;
        if (!(std::cin >> cOrigen >> fDestino >> cDestino)) {
            std::cout << "no se puede hacer este movimiento. Intente de nuevo.\n";
            
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        Position desde{fOrigen, cOrigen};
        Position hasta{fDestino, cDestino};

        if (!tablero.inBounds(desde.fila, desde.columna) || !tablero.inBounds(hasta.fila, hasta.columna)) {
            std::cout << "Coordenadas fuera de rango.\n";
            continue;
        }
        if (!tablero.get(desde.fila, desde.columna).has_value()) {
            std::cout << "No hay pieza en el origen.\n";
            continue;
        }
        if (tablero.get(desde.fila, desde.columna)->propietario != jugadorActual) {
            std::cout << "Esa pieza no pertenece al jugador actual.\n";
            continue;
        }

        bool movido = false;
        if (capturaObligatoria || !tablero.getCaptureDestinationsFrom(desde.fila, desde.columna).empty()) {
            movido = applyCaptureMove(desde, hasta, resumen, capturasEncadenadas);
            if (!movido) {
                std::cout << "Movimiento de captura invalido.\n";
                continue;
            }
            resumen += "(" + std::to_string(desde.fila) + "," + std::to_string(desde.columna) + ")x->(" + std::to_string(hasta.fila) + "," + std::to_string(hasta.columna) + ") ";
            
            if (!tablero.get(hasta.fila, hasta.columna).has_value()) {
                std::cout << "Error interno tras mover.\n";
                return false;
            }
            auto masCapturas = tablero.getCaptureDestinationsFrom(hasta.fila, hasta.columna);
            if (!masCapturas.empty()) {
                std::cout << "Captura multiple disponible; continue capturando con la misma pieza.\n";
                
                capturaObligatoria = true;
                
                
                desde = hasta;
                continue; 
            } else {
                
                break;
            }
        } else {
            
            movido = applyNormalMove(desde, hasta, resumen);
            if (!movido) {
                std::cout << "Movimiento invalido.\n";
                continue;
            }
            resumen += "(" + std::to_string(desde.fila) + "," + std::to_string(desde.columna) + ") -> (" + std::to_string(hasta.fila) + "," + std::to_string(hasta.columna) + ") ";
            break;
        }
    }

    conteoMovimientos++;
    historial.push_back(resumen);

    std::cout << "\n" << resumen << "\n";
    std::cout << "Historial (ultimos 10):\n";
    for (int i = std::max(0, (int)historial.size() - 10); i < (int)historial.size(); ++i) {
        std::cout << "- " << historial[i] << "\n";
    }
    return true;
}

void Game::showSuggestion() const {
    bool hayCaptura = tablero.hasAnyCapture(jugadorActual);
    if (hayCaptura) {
        std::cout << "Sugerencia de captura obligatoria:\n";
        for (int f = 0; f < 10; ++f) {
            for (int c = 0; c < 10; ++c) {
                auto p = tablero.get(f, c);
                if (p.has_value() && p->propietario == jugadorActual) {
                    auto dests = tablero.getCaptureDestinationsFrom(f, c);
                    for (auto d : dests) {
                        std::cout << "- (" << f << "," << c << ") x-> (" << d.fila << "," << d.columna << ")";
                        Board sim = tablero;
                        int fCap = f + (d.fila - f)/2;
                        int cCap = c + (d.columna - c)/2;
                        sim.remove(fCap, cCap);
                        sim.set(d.fila, d.columna, p);
                        sim.remove(f, c);
                        auto mas = sim.getCaptureDestinationsFrom(d.fila, d.columna);
                        if (!mas.empty()) std::cout << " (multiple disponible)";
                        std::cout << "\n";
                        return;
                    }
                }
            }
        }
    } else {
        std::cout << "Sugerencia de movimiento normal:\n";
        for (int f = 0; f < 10; ++f) {
            for (int c = 0; c < 10; ++c) {
                auto p = tablero.get(f, c);
                if (p.has_value() && p->propietario == jugadorActual) {
                    auto dests = tablero.getNormalDestinationsFrom(f, c);
                    for (auto d : dests) {
                        std::cout << "- (" << f << "," << c << ") -> (" << d.fila << "," << d.columna << ")\n";
                        return;
                    }
                }
            }
        }
        std::cout << "No hay movimientos normales disponibles.\n";
    }
}