#pragma once

namespace CHESS{
    enum Piece {PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING, NONE};
    enum Player {NONE, WHITE, BLACK};
    enum Erreur {AUCUN, OUTSIDE_BOARD, NO_PIECE, OTHER_PLAYER_PIECE, EATING_OWN_PIECE};

    struct SinglePosition{
        Piece piece;
        Player player;
        bool isEmpty() { return piece == Piece::NONE; }
        void setEmpty() {piece == Piece::NONE; player = Player::NONE; }
        void setPos(SinglePosition copy) {piece = copy.piece; player = copy.player; }
    };

    struct BoardSquare {
        int row; // 0–7
        int col; // 0–7
        SinglePosition pos;
    };

    struct MovePieceResult {
        bool isSuccess;
        Erreur erreur;
        bool setErreur(Erreur erreurP) { isSuccess = false; erreur=erreurP; }
        bool setSuccess() { isSuccess = true; erreur=Erreur::AUCUN; }
    };

    // La colonne est une lettre entre A-H et row entre 1-8
    MovePieceResult movePiece(Player player, int fromCol, int fromRow, int toCol, int toRow);
    void setupBoard();
    char convertIntToCol(int value);
    int convertColToInt(char col);
    bool isInsideBoard(int row, int col);
}