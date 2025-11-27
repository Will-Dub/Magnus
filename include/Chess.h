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

    struct MovePieceResult {
        int fromCol, fromRow;
        int toCol, toRow;
        bool isSuccess;
        Erreur erreur;
        bool isPawnOnDest;
        Player player;
        bool isGameLost;
        void setErreur(Erreur erreurP) { isSuccess = false; erreur=erreurP; }
        void setSuccess() { isSuccess = true; erreur=Erreur::AUCUN; }
    };

    struct MinimaxMove {
        int fromRow, fromCol;
        int toRow, toCol;
        int score;
    };

    const int MAX_MOVES = 256;

    // La colonne est une lettre entre A-H et row entre 1-8
    MovePieceResult movePiece(int fromCol, int fromRow, int toCol, int toRow);
    void setupBoard();
    bool isInsideBoard(int col, int row);
    void printBoard();
    void printPiece(int col, int row);
    void printCurrentPlayer();
    String getErrorMessage(Erreur erreur);
    void switchTurn();
    Player getCurrentTurn();
    bool isKingInCheck(Player player);
    bool isMoveValidForPiece(const Square& fromSquare, 
                         int toCol, int toRow,
                         bool targetHasEnemy);
    int pieceValue(Piece p);
    int evaluateBoard();
    int generateMoves(Player player, MinimaxMove moveList[]);
    int minimax(int depth, int alpha, int beta, Player player);
    MinimaxMove findBestMove(int depth);
}
