#include "Chess.h"

namespace CHESS
{
    BoardSquare board[8][8];
    Player currentTurnPlayer;

    /**
     * @brief Initialise le board
     * 
     */
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

    /**
     * @brief Fait un move sur le board
     * 
     * @param fromCol index col de la position initial (0-7)
     * @param fromRow index row de la position initial (0-7)
     * @param toCol index col de la position final (0-7)
     * @param toRow index col de la position final (0-7)
     */
    MovePieceResult movePiece(int fromCol, int fromRow, int toCol, int toRow){
        MovePieceResult result {};
        result.player = currentTurnPlayer;

        // Affiche le move entré
        char selectedFromColChar = 'A' + fromCol;
        int selectedFromRow = fromRow + 1;
        char selectedToColChar = 'A' + toCol;
        int selectedToRow = toRow + 1;

        Serial.print("Mouvement entré: ");
        String textMove = "["+String(selectedFromColChar)+"]" + "["+String(selectedFromRow)+"] à ["+String(selectedToColChar)+"]" + "["+String(selectedToRow)+"]";
        Serial.println(textMove);

        // Check if the index are in the 0-7 range
        if(!isInsideBoard(fromRow, fromCol) || !isInsideBoard(toRow, toCol)){
            result.setErreur(Erreur::OUTSIDE_BOARD);
            return result;
        }

        // Store piece position
        SinglePosition fromPosition = board[fromRow][fromCol].pos;
        SinglePosition toPosition = board[toRow][toCol].pos;
        bool isPawnOnDest = !toPosition.isEmpty();

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
        result.isPawnOnDest = isPawnOnDest;
        return result;
    }

    bool isPathClear(int fromRow, int fromCol, int toRow, int toCol) {
        int dRow = (toRow - fromRow);
        int dCol = (toCol - fromCol);

        int stepRow = (dRow == 0 ? 0 : (dRow > 0 ? 1 : -1));
        int stepCol = (dCol == 0 ? 0 : (dCol > 0 ? 1 : -1));

        int r = fromRow + stepRow;
        int c = fromCol + stepCol;

        while (r != toRow || c != toCol) {
            if (!board[r][c].pos.isEmpty()) return false;
            r += stepRow;
            c += stepCol;
        }
        return true;
    }

    bool isMoveValidForPiece(const BoardSquare& fromSquare, 
                         int toRow, int toCol,
                         bool targetHasEnemy)
    {
        const int FROM_ROW = fromSquare.row;
        const int FROM_COL = fromSquare.col;
        const SinglePosition& fromPosition = fromSquare.pos;
        int dRow = toRow - fromSquare.row;
        int dCol = toCol - fromSquare.col;

        switch (fromSquare.pos.piece) {
        case Piece::PAWN: {
            int direction = (fromPosition.player == Player::WHITE ? 1 : -1);
            int startRow  = (fromPosition.player == Player::WHITE ? 1 : 6);

            // Move forward 1
            if (dCol == 0 && dRow == direction && !targetHasEnemy)
                return true;

            // Move forward 2 from starting row
            if (dCol == 0 && dRow == 2 * direction && FROM_ROW == startRow) {
                int midRow = FROM_ROW + direction;
                if (board[midRow][FROM_COL].pos.isEmpty() && !targetHasEnemy)
                    return true;
            }

            // Capture diagonally
            if (abs(dCol) == 1 && dRow == direction && targetHasEnemy)
                return true;

            return false;
        }

        case Piece::ROOK:
            if (dRow == 0 || dCol == 0)
                return isPathClear(FROM_ROW, FROM_COL, toRow, toCol);
            return false;

        case Piece::KNIGHT:
            if ((abs(dRow) == 2 && abs(dCol) == 1) ||
                (abs(dRow) == 1 && abs(dCol) == 2))
                return true;
            return false;

        case Piece::BISHOP:
            if (abs(dRow) == abs(dCol))
                return isPathClear(FROM_ROW, FROM_COL, toRow, toCol);
            return false;

        case Piece::QUEEN:
            if (dRow == 0 || dCol == 0 || abs(dRow) == abs(dCol))
                return isPathClear(FROM_ROW, FROM_COL, toRow, toCol);
            return false;

        case Piece::KING:
            if (abs(dRow) <= 1 && abs(dCol) <= 1)
                return true;
            return false;

        default:
            return false;
        }
    }

    /**
     * @brief Check if a piece is in the board
     * 
     * @param row row to check
     * @param col col to check
     * @return true if the position is valid
     * @return false if the position is invalid
     */
    bool isInsideBoard(int row, int col) {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }

    /**
     * @brief Serial print the board
     */
    void printBoard() {
        Serial.println();
        for (int r = 7; r >= 0; --r) {
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

    /**
     * @brief Serial print an individual piece
     * 
     * @param col of the piece
     * @param row of the piece
     */
    void printPiece(int col, int row){
        SinglePosition position = board[row][col].pos;
        Piece piece = position.piece;

        switch (position.player)
        {
        case Player::WHITE:
            //Serial.print("\033[37m");
            break;
        
        case Player::BLACK:
            //Serial.print("\033[33m");
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

        //Serial.print("\033[0m");
    }

    /**
     * @brief Serial print the current player
     * 
     */
    void printCurrentPlayer(){
        Serial.print("Au tour de: ");
        Serial.println(currentTurnPlayer == Player::WHITE ? "Blanc" : "Noir");
        //Serial.println(currentTurnPlayer == Player::WHITE ? "\033[37mBlanc" : "\033[33mNoir");
        //Serial.print("\033[0m");
    }

    /**
     * @brief Get the error message string
     * 
     * @param erreur to get the string from
     * @return String of the error
     */
    String getErrorMessage(Erreur erreur){
        switch (erreur) {
            case Erreur::OUTSIDE_BOARD:
                return "Mouvement en dehors de la planche";
            case Erreur::NO_PIECE:
                return "Il n'y a aucune pièce à cet endroit";
            case Erreur::OTHER_PLAYER_PIECE:
                return "Le pion ne vous appartient pas";
            case Erreur::EATING_OWN_PIECE:
                return "Vous ne pouvez pas manger votre propre pion";
            default:
                return "Erreur inconnue";
        }
    }

    /**
     * @brief Change the current turn
     */
    void switchTurn(){
        if(currentTurnPlayer == Player::WHITE){
            currentTurnPlayer = Player::BLACK;
        }else{
            currentTurnPlayer = Player::WHITE;
        }
    }

    Player getCurrentTurn(){
        return currentTurnPlayer;
    }
}