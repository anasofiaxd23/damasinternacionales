#include "Board.h"
#include "colores.h"
#include <iomanip>

Board::Board() : cuadricula(10, std::vector<std::optional<Piece>>(10, std::nullopt)) {}

bool Board::inBounds(int fila, int columna) const {
    return fila >= 0 && fila < 10 && columna >= 0 && columna < 10;
}

bool Board::isPlayable(int fila, int columna) const {
    return inBounds(fila, columna) && ((fila + columna) % 2 == 1);
}

const std::optional<Piece>& Board::get(int fila, int columna) const {
    return cuadricula[fila][columna];
}

void Board::set(int fila, int columna, const std::optional<Piece>& pieza) {
    cuadricula[fila][columna] = pieza;
}

void Board::remove(int fila, int columna) {
    cuadricula[fila][columna] = std::nullopt;
}

void Board::setupInitial() {
    
    for (int fila = 0; fila < 4; ++fila) {
        for (int columna = 0; columna < 10; ++columna) {
            if (isPlayable(fila, columna)) {
                cuadricula[fila][columna] = Piece{PieceType::MAN, Player::PLAYER2};
            }
        }
    }
    for (int fila = 6; fila < 10; ++fila) {
        for (int columna = 0; columna < 10; ++columna) {
            if (isPlayable(fila, columna)) {
                cuadricula[fila][columna] = Piece{PieceType::MAN, Player::PLAYER1};
            }
        }
    }
}

void Board::draw() const {
    std::cout << "  |";
    for (int columna = 0; columna < 10; ++columna) std::cout << std::setw(2) << columna;
    std::cout << "\n";

    for (int fila = 0; fila < 10; ++fila) {
        std::cout << std::setw(2) << fila << " |";
        for (int columna = 0; columna < 10; ++columna) {
            bool oscura = ((fila + columna) % 2) != 0;
            if (oscura) fondoOscuro(); else fondoClaro();
            if (cuadricula[fila][columna].has_value()) {
                char s = pieceSymbol(*cuadricula[fila][columna]);
                if (s == 'x' || s == 'X') {
                    colorRojo();
                    std::cout << s << ' ';
                } else if (s == 'o' || s == 'O') {
                    colorAzul();
                    std::cout << s << ' ';
                } else {
                    std::cout << s << ' ';
                }
                resetColor();
            } else {
                std::cout << "  ";
                resetColor();
            }
        }
        std::cout << "|" << std::setw(2) << fila << "\n";
    }
    std::cout << "  |";
    for (int columna = 0; columna < 10; ++columna) std::cout << std::setw(2) << columna;
    std::cout << "\n";
}

std::vector<Position> Board::getNormalDestinationsFrom(int fila, int columna) const {
    std::vector<Position> dests;
    if (!inBounds(fila, columna) || !cuadricula[fila][columna].has_value()) return dests;
    const Piece& p = *cuadricula[fila][columna];
    int direcciones[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};

    if (p.tipo == PieceType::MAN) {
        int avance = (p.propietario == Player::PLAYER2) ? 1 : -1;
        for (int dc : {-1, 1}) {
            int nf = fila + avance;
            int nc = columna + dc;
            if (isPlayable(nf, nc) && !cuadricula[nf][nc].has_value()) dests.push_back({nf, nc});
        }
    } else {
        for (auto& d : direcciones) {
            int nf = fila + d[0];
            int nc = columna + d[1];
            if (isPlayable(nf, nc) && !cuadricula[nf][nc].has_value()) dests.push_back({nf, nc});
        }
    }
    return dests;
}

std::vector<Position> Board::getCaptureDestinationsFrom(int fila, int columna) const {
    std::vector<Position> dests;
    if (!inBounds(fila, columna) || !cuadricula[fila][columna].has_value()) return dests;
    const Piece& p = *cuadricula[fila][columna];

    if (p.tipo == PieceType::MAN) {
        int avance = (p.propietario == Player::PLAYER2) ? 1 : -1;
        for (int dc : {-1, 1}) {
            int mf = fila + avance;
            int mc = columna + dc;
            int lf = fila + 2*avance;
            int lc = columna + 2*dc;
            if (!isPlayable(lf, lc)) continue;
            if (inBounds(mf, mc) && cuadricula[mf][mc].has_value() && cuadricula[mf][mc]->propietario != p.propietario) {
                if (!cuadricula[lf][lc].has_value()) dests.push_back({lf, lc});
            }
        }
    } else {
        int direcciones[4][2] = {{1,1},{1,-1},{-1,1},{-1,-1}};
        for (auto& d : direcciones) {
            int mf = fila + d[0];
            int mc = columna + d[1];
            int lf = fila + 2*d[0];
            int lc = columna + 2*d[1];
            if (!isPlayable(lf, lc)) continue;
            if (inBounds(mf, mc) && cuadricula[mf][mc].has_value() && cuadricula[mf][mc]->propietario != p.propietario) {
                if (!cuadricula[lf][lc].has_value()) dests.push_back({lf, lc});
            }
        }
    }
    return dests;
}

bool Board::hasAnyCapture(Player jugador) const {
    for (int fila = 0; fila < 10; ++fila) {
        for (int columna = 0; columna < 10; ++columna) {
            if (cuadricula[fila][columna].has_value() && cuadricula[fila][columna]->propietario == jugador) {
                auto caps = getCaptureDestinationsFrom(fila, columna);
                if (!caps.empty()) return true;
            }
        }
    }
    return false;
}

bool Board::hasAnyMoves(Player jugador) const {
    for (int fila = 0; fila < 10; ++fila) {
        for (int columna = 0; columna < 10; ++columna) {
            if (cuadricula[fila][columna].has_value() && cuadricula[fila][columna]->propietario == jugador) {
                if (!getCaptureDestinationsFrom(fila, columna).empty()) return true;
                if (!getNormalDestinationsFrom(fila, columna).empty()) return true;
            }
        }
    }
    return false;
}