#pragma once

#include <Arduino.h>

namespace CHESS{
    enum class Piece {PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING, NONE};
    enum class Player {NONE, WHITE, BLACK};
    enum class Erreur {AUCUN, OUTSIDE_BOARD, NO_PIECE, OTHER_PLAYER_PIECE, EATING_OWN_PIECE, INVALID_MOVE, KING_CHECK};

    struct Position{
        Piece piece;
        Player player;
        bool isEmpty() { return piece == Piece::NONE; }
        void setEmpty() {piece = Piece::NONE; player = Player::NONE; }
        void setPosition(Position copy) {piece = copy.piece; player = copy.player; }
    };

    struct Square {
        uint8_t row; // 0–7
        uint8_t col; // 0–7
        Position position;
    };

    struct MovePieceResult {
        bool isSuccess;
        Erreur erreur;
        bool isPawnOnDest;
        Player player;
        void setErreur(Erreur erreurP) { isSuccess = false; erreur=erreurP; }
        void setSuccess() { isSuccess = true; erreur=Erreur::AUCUN; }
    };

    // La colonne est une lettre entre A-H et row entre 1-8
    MovePieceResult movePiece(int fromCol, int fromRow, int toCol, int toRow);
    void setupBoard();
    bool isInsideBoard(int row, int col);
    void printBoard();
    void printPiece(int col, int row);
    void printCurrentPlayer();
    String getErrorMessage(Erreur erreur);
    void switchTurn();
    Player getCurrentTurn();
    bool isKingInCheck(Player player);
    bool isMoveValidForPiece(const Square& fromSquare, 
                         int toRow, int toCol,
                         bool targetHasEnemy);
    
}