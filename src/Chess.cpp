#include "Chess.h"

namespace CHESS
{
    CHESS::BoardSquare board[8][8];

    void setupBoard(){
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
                board[r][c] = { r, c, { Piece::NONE, Player::NONE } };

        // Pawns
        for (int c = 0; c < 8; ++c) {
            board[1][c].pos = { Piece::PAWN, Player::WHITE };
            board[6][c].pos = { Piece::PAWN, Player::BLACK };
        }

        // Rooks
        board[0][0].pos = { Piece::ROOK, Player::WHITE };
        board[0][7].pos = { Piece::ROOK, Player::WHITE };
        board[7][0].pos = { Piece::ROOK, Player::BLACK };
        board[7][7].pos = { Piece::ROOK, Player::BLACK };

        // Knights
        board[0][1].pos = { Piece::KNIGHT, Player::WHITE };
        board[0][6].pos = { Piece::KNIGHT, Player::WHITE };
        board[7][1].pos = { Piece::KNIGHT, Player::BLACK };
        board[7][6].pos = { Piece::KNIGHT, Player::BLACK };

        // Bishops
        board[0][2].pos = { Piece::BISHOP, Player::WHITE };
        board[0][5].pos = { Piece::BISHOP, Player::WHITE };
        board[7][2].pos = { Piece::BISHOP, Player::BLACK };
        board[7][5].pos = { Piece::BISHOP, Player::BLACK };

        // Queens
        board[0][3].pos = { Piece::QUEEN, Player::WHITE };
        board[7][3].pos = { Piece::QUEEN, Player::BLACK };

        // Kings
        board[0][4].pos = { Piece::KING, Player::WHITE };
        board[7][4].pos = { Piece::KING, Player::BLACK };
    }

    MovePieceResult movePiece(Player player, int fromCol, int fromRow, int toCol, int toRow){
        MovePieceResult result {};

        // Check si les position sont dans le range 0-8
        if(!isInsideBoard(fromRow, fromCol) || !isInsideBoard(toRow, toCol)){
            result.setErreur(Erreur::OUTSIDE_BOARD);
            return result;
        }

        SinglePosition fromPosition = board[fromRow][fromCol].pos;
        SinglePosition toPosition = board[toRow][toCol].pos;

        // Check if there is a piece, from pos
        if(fromPosition.isEmpty()){
            result.setErreur(Erreur::NO_PIECE);
            return result;
        }

        // Check if the piece is from the player
        if(fromPosition.player != player){
            result.setErreur(Erreur::OTHER_PLAYER_PIECE);
            return result;
        }

        if(toPosition.player == player){
            result.setErreur(Erreur::EATING_OWN_PIECE);
            return result;
        }

        // TODO: check si le player est échec
        // TODO: Check if the move is valid specific to pawn type

        // Execute the move
        board[fromRow][fromCol].pos.setEmpty();
        board[toRow][toCol].pos.setPos(fromPosition);

        result.setSuccess();
        return result;
    }

    bool isInsideBoard(int row, int col) {
        // Row et col entre 1-8 inclusivement
        return row > 0 && row <= 8 && col > 0 && col <= 8;
    }
}