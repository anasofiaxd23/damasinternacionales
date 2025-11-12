#pragma once
#include <string>

enum class PieceType { MAN, KING };
enum class Player { NONE, PLAYER1, PLAYER2 };

struct Piece {
    PieceType tipo;
    Player propietario;
};

inline char pieceSymbol(const Piece& pieza) {
    if (pieza.propietario == Player::PLAYER1) {
        return pieza.tipo == PieceType::MAN ? 'x' : 'X';
    } else if (pieza.propietario == Player::PLAYER2) {
        return pieza.tipo == PieceType::MAN ? 'o' : 'O';
    }
    return ' ';
}

inline std::string playerName(Player jugador) {
    switch (jugador) {
        case Player::PLAYER1: return "Jugador X";
        case Player::PLAYER2: return "Jugador O";
        default: return "Ninguno";
    }
}
