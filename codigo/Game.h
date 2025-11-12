#pragma once
#include <vector>
#include <string>
#include "Board.h"
#include "Piece.h"

class Game {
public:
    Game();
    void run();

private:
    Board tablero;
    Player jugadorActual;
    int conteoMovimientos;
    int puntajeJ1;
    int puntajeJ2;
    std::vector<std::string> historial;
    bool enEjecucion;

    void showMenu();
    void showRules() const;
    void startTwoPlayers();
    void switchTurn();
    void showSuggestion() const;

    bool takeTurn();
    bool applyNormalMove(const Position& desde, const Position& hasta, std::string& resumen);
    bool applyCaptureMove(const Position& desde, const Position& hasta, std::string& resumen, int& capturas);

    bool shouldPromote(Player jugador, int fila) const;
};