#pragma once
#include <vector>
#include <optional>
#include <utility>
#include <iostream>
#include "Piece.h"

struct Position {
    int fila;
    int columna;
};

class Board {
public:
    Board();
    void setupInitial();
    void draw() const;

    bool inBounds(int fila, int columna) const;
    bool isPlayable(int fila, int columna) const;

    const std::optional<Piece>& get(int fila, int columna) const;
    void set(int fila, int columna, const std::optional<Piece>& pieza);
    void remove(int fila, int columna);

    bool hasAnyCapture(Player jugador) const;
    bool hasAnyMoves(Player jugador) const;

    std::vector<Position> getCaptureDestinationsFrom(int fila, int columna) const;
    std::vector<Position> getNormalDestinationsFrom(int fila, int columna) const;

private:
    std::vector<std::vector<std::optional<Piece>>> cuadricula;
};
