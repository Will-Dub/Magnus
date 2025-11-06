#include "Chess.h"

namespace CHESS
{
    BoardSquare board[8][8];
    Player currentTurnPlayer;

    void setupBoard(){
        currentTurnPlayer = Player::WHITE;

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

    MovePieceResult movePiece(int fromCol, int fromRow, int toCol, int toRow){
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
        if(fromPosition.player != currentTurnPlayer){
            result.setErreur(Erreur::OTHER_PLAYER_PIECE);
            return result;
        }

        if(toPosition.player == currentTurnPlayer){
            result.setErreur(Erreur::EATING_OWN_PIECE);
            return result;
        }

        // TODO: check si le player est échec
        // TODO: Check if the move is valid specific to pawn type

        // Execute the move
        board[fromRow][fromCol].pos.setEmpty();
        board[toRow][toCol].pos.setPos(fromPosition);

        // Change current player
        switchTurn();

        result.setSuccess();
        return result;
    }

    bool isInsideBoard(int row, int col) {
        // Row et col entre 1-8 inclusivement
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    void printBoard() {
        for (int r = 0; r < 8; ++r) {
            Serial.print(r + 1);
            Serial.print(" ");

            for (int c = 0; c < 8; ++c) {
                printPiece(c, r);
            }
            Serial.println();
        }

        Serial.print("  ");
        for (char c = 'a'; c <= 'h'; ++c) {
            Serial.print(" ");
            Serial.print(c);
        }
        Serial.println();
        Serial.println();
    }

    void printPiece(int col, int row){
        SinglePosition position = board[row][col].pos;
        Piece piece = position.piece;

        switch (position.player)
        {
        case Player::WHITE:
            Serial.print("\033[37m");
            break;
        
        case Player::BLACK:
            Serial.print("\033[30m");
            break;
        
        default:
            //Serial.print("\033[33m");
            break;
        }

        Serial.print(" ");
        switch(piece){
        case Piece::NONE:
            Serial.print(" ");
            break;
        case Piece::PAWN:
            Serial.print("♙");
            break;
        case Piece::BISHOP:
            Serial.print("♗");
            break;
        case Piece::KNIGHT:
            Serial.print("♘");
            break;
        case Piece::ROOK:
            Serial.print("♖");
            break;
        case Piece::QUEEN:
            Serial.print("♕");
            break;
        case Piece::KING:
            Serial.print("♔");
            break;
        default:
            break;
        }

        Serial.print("\033[0m");
    }

    void printCurrentPlayer(){
        switch (currentTurnPlayer) {
            case Player::WHITE:
                Serial.println("Blanc: ");
                break;
            case Player::BLACK:
                Serial.println("Noir: ");
                break;
            default:
                Serial.println(" ");
                break;
        }
    }

    void printError(Erreur erreur){
        switch (erreur) {
            case Erreur::OUTSIDE_BOARD:
                Serial.println("Mouvement en dehors de la planche");
                break;
            case Erreur::NO_PIECE:
                Serial.println("Il n'y a aucune pièce à cet endroit");
                break;
            case Erreur::OTHER_PLAYER_PIECE:
                Serial.println("Le pion ne vous appartient pas");
                break;
            case Erreur::EATING_OWN_PIECE:
                Serial.println("Vous ne pouvez pas manger votre propre pion");
                break;
            default:
                Serial.println("Erreur inconnue");
                break;
        }
    }

    void switchTurn(){
        if(currentTurnPlayer == Player::WHITE){
            currentTurnPlayer = Player::BLACK;
        }else{
            currentTurnPlayer = Player::WHITE;
        }
    }
}