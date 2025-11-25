#include "Chess.h"

namespace CHESS
{
    Square board[8][8];
    Player currentTurnPlayer;

    /**
     * @brief Initialise le board
     * 
     */
    void setupBoard(){
        currentTurnPlayer = Player::WHITE;

        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
                board[r][c] = { (uint8_t)r, (uint8_t)c, { Piece::NONE, Player::NONE } };

        // Pawns
        for (int c = 0; c < 8; ++c) {
            board[1][c].position = { Piece::PAWN, Player::WHITE };
            board[6][c].position = { Piece::PAWN, Player::BLACK };
        }

        // Rooks
        board[0][0].position = { Piece::ROOK, Player::WHITE };
        board[0][7].position = { Piece::ROOK, Player::WHITE };
        board[7][0].position = { Piece::ROOK, Player::BLACK };
        board[7][7].position = { Piece::ROOK, Player::BLACK };

        // Knights
        board[0][1].position = { Piece::KNIGHT, Player::WHITE };
        board[0][6].position = { Piece::KNIGHT, Player::WHITE };
        board[7][1].position = { Piece::KNIGHT, Player::BLACK };
        board[7][6].position = { Piece::KNIGHT, Player::BLACK };

        // Bishops
        board[0][2].position = { Piece::BISHOP, Player::WHITE };
        board[0][5].position = { Piece::BISHOP, Player::WHITE };
        board[7][2].position = { Piece::BISHOP, Player::BLACK };
        board[7][5].position = { Piece::BISHOP, Player::BLACK };

        // Queens
        board[0][3].position = { Piece::QUEEN, Player::WHITE };
        board[7][3].position = { Piece::QUEEN, Player::BLACK };

        // Kings
        board[0][4].position = { Piece::KING, Player::WHITE };
        board[7][4].position = { Piece::KING, Player::BLACK };
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
        result.isGameLost = false;

        // Affiche le move entré
        char selectedFromColChar = 'A' + fromCol;
        int selectedFromRow = fromRow + 1;
        char selectedToColChar = 'A' + toCol;
        int selectedToRow = toRow + 1;

        Serial.print("Mouvement entré: ");
        String textMove = "["+String(selectedFromColChar)+"]" + "["+String(selectedFromRow)+"] à ["+String(selectedToColChar)+"]" + "["+String(selectedToRow)+"]";
        Serial.println(textMove);

        // Check if the index are in the 0-7 range
        if(!isInsideBoard(fromCol, fromRow) || !isInsideBoard(toCol, toRow)){
            result.setErreur(Erreur::OUTSIDE_BOARD);
            return result;
        }

        // Store piece position
        Position fromPosition = board[fromRow][fromCol].position;
        Position toPosition = board[toRow][toCol].position;
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

        // Check spécifique pour le type de pion
        if(!isMoveValidForPiece(board[fromRow][fromCol], toCol, toRow, !toPosition.isEmpty())){
            result.setErreur(Erreur::INVALID_MOVE);
            return result;
        }

        // Check si joueur est échec
        Position savedFrom = fromPosition;
        Position savedTo = toPosition;

        // Exécute le mouvement
        board[fromRow][fromCol].position.setEmpty();
        board[toRow][toCol].position.setPosition(savedFrom);

        // Vérifie si le roi est échec
        if (isKingInCheck(currentTurnPlayer)) {
            // Restore avant le mouvement
            board[fromRow][fromCol].position.setPosition(savedFrom);
            board[toRow][toCol].position = savedTo;

            result.setErreur(Erreur::KING_CHECK);
            return result;
        }

        switchTurn();
        result.setSuccess();
        result.isPawnOnDest = isPawnOnDest;
        result.fromCol = fromCol;
        result.fromRow = fromRow;
        result.toCol = toCol;
        result.toRow = toRow;
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
            if (!board[r][c].position.isEmpty()) return false;
            r += stepRow;
            c += stepCol;
        }
        return true;
    }

    bool isMoveValidForPiece(const Square& fromSquare, 
                         int toCol, int toRow,
                         bool targetHasEnemy)
    {
        const int FROM_ROW = fromSquare.row;
        const int FROM_COL = fromSquare.col;
        const Position& fromPosition = fromSquare.position;
        int dRow = toRow - fromSquare.row;
        int dCol = toCol - fromSquare.col;

        if (fromSquare.col == toCol && fromSquare.row == toRow) return false;
        if (fromSquare.position.player == board[toRow][toCol].position.player) return false;

        switch (fromSquare.position.piece) {
        case Piece::PAWN: {
            int direction = (fromPosition.player == Player::WHITE ? 1 : -1);
            int startRow  = (fromPosition.player == Player::WHITE ? 1 : 6);

            // Avance de 1
            if (dCol == 0 && dRow == direction && !targetHasEnemy)
                return true;

            // Avance de 2
            if (dCol == 0 && dRow == 2 * direction && FROM_ROW == startRow) {
                int midRow = FROM_ROW + direction;
                if (board[midRow][FROM_COL].position.isEmpty() && !targetHasEnemy)
                    return true;
            }

            // Diago
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

    bool isKingInCheck(Player player)
    {
        int kingRow = -1, kingCol = -1;

        // Trouver le roi
        for(int r = 0; r < 8; r++){
            for(int c = 0; c < 8; c++){
                if(!board[r][c].position.isEmpty() &&
                board[r][c].position.piece == Piece::KING &&
                board[r][c].position.player == player)
                {
                    kingRow = r;
                    kingCol = c;
                }
            }
        }

        // Vérifier si une pièce ennemie peut attaquer
        for(int r = 0; r < 8; r++){
            for(int c = 0; c < 8; c++){
                if(!board[r][c].position.isEmpty() && board[r][c].position.player != player)
                {
                    bool targetHasEnemy = true;

                    if (isMoveValidForPiece(board[r][c], kingCol, kingRow, targetHasEnemy))
                    {
                        return true;
                    }
                }
            }
        }

        return false;
    }

    /**
     * @brief Check if a piece is in the board
     * 
     * @param col col to check
     * @param row row to check
     * @return true if the position is valid
     * @return false if the position is invalid
     */
    bool isInsideBoard(int col, int row) {
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
        Position position = board[row][col].position;
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
            case Erreur::INVALID_MOVE:
                return "Le mouvement avec le pion est invalide";
            case Erreur::KING_CHECK:
                return "Vous êtes échec";
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

    int pieceValue(Piece p) {
        switch (p) {
            case Piece::PAWN:   return 100;
            case Piece::KNIGHT: return 320;
            case Piece::BISHOP: return 330;
            case Piece::ROOK:   return 500;
            case Piece::QUEEN:  return 900;
            case Piece::KING:   return 20000;
            default: return 0;
        }
    }

    /**
     * Évalue qui à l'avantage
     */
    int evaluateBoard() {
        int score = 0;
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Position pos = board[r][c].position;
                if (!pos.isEmpty()) {
                    int val = pieceValue(pos.piece);
                    if(pos.player == Player::WHITE){
                        score += val;
                    }else{
                        score -= val;
                    }
                }
            }
        }
        return score;
    }

    /**
     * Génère la liste des mouvements
     */
    int generateMoves(Player player, MinimaxMove moveList[]) {
        int count = 0;

        // Pour les 32 carré
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Position pos = board[r][c].position;

                if (pos.isEmpty() || pos.player != player)
                    continue;

                // Essaie tous les mouvement possible
                for (int tr = 0; tr < 8; tr++) {
                    for (int tc = 0; tc < 8; tc++) {
                        Position target = board[tr][tc].position;
                        bool targetHasEnemy = !target.isEmpty() && target.player != player;

                        if (!isMoveValidForPiece(board[r][c], tc, tr, targetHasEnemy))
                            continue;

                        // Simule
                        Position savedFrom = pos;
                        Position savedTo = target;

                        board[r][c].position.setEmpty();
                        board[tr][tc].position = savedFrom;

                        bool illegal = isKingInCheck(player);

                        // Revert
                        board[r][c].position = savedFrom;
                        board[tr][tc].position = savedTo;

                        if (!illegal) {
                            moveList[count].fromRow = r;
                            moveList[count].fromCol = c;
                            moveList[count].toRow = tr;
                            moveList[count].toCol = tc;
                            count++;

                            if (count >= MAX_MOVES)
                                return count;
                        }
                    }
                }
            }
        }
        return count;
    }

    /**
     * Fonction minimax pour évaluer une branche
     */
    int minimax(int depth, int alpha, int beta, Player player) {
        if (depth == 0)
            return evaluateBoard();

        MinimaxMove moves[MAX_MOVES];
        int moveCount = generateMoves(player, moves);

        if (moveCount == 0)
            return evaluateBoard();

        bool maximizing = (player == Player::WHITE);
        int bestScore = maximizing ? -30000 : 30000;

        for (int i = 0; i < moveCount; i++) {
            MinimaxMove m = moves[i];

            // Simule
            Position savedFrom = board[m.fromRow][m.fromCol].position;
            Position savedTo = board[m.toRow][m.toCol].position;

            board[m.fromRow][m.fromCol].position.setEmpty();
            board[m.toRow][m.toCol].position = savedFrom;

            int score = minimax(
                depth - 1,
                alpha,
                beta,
                (player == Player::WHITE ? Player::BLACK : Player::WHITE)
            );

            // Revert
            board[m.fromRow][m.fromCol].position = savedFrom;
            board[m.toRow][m.toCol].position = savedTo;

            if (maximizing) {
                if (score > bestScore) bestScore = score;
                if (score > alpha) alpha = score;
            } else {
                if (score < bestScore) bestScore = score;
                if (score < beta) beta = score;
            }

            if (beta <= alpha)
                break;
        }

        return bestScore;
    }

    /** 
     * Retourne le meilleur mouvement
     */
    MinimaxMove findBestMove(int depth) {
        Player player = currentTurnPlayer;

        MinimaxMove moves[MAX_MOVES];
        int moveCount = generateMoves(player, moves);

        MinimaxMove bestMove;
        bestMove.score = (player == Player::WHITE ? -30000 : 30000);

        bool maximizing = (player == Player::WHITE);

        for (int i = 0; i < moveCount; i++) {
            MinimaxMove m = moves[i];

            // Simule
            Position savedFrom = board[m.fromRow][m.fromCol].position;
            Position savedTo = board[m.toRow][m.toCol].position;

            board[m.fromRow][m.fromCol].position.setEmpty();
            board[m.toRow][m.toCol].position = savedFrom;

            int score = minimax(
                depth - 1,
                -30000,
                30000,
                (player == Player::WHITE ? Player::BLACK : Player::WHITE)
            );

            // Revert
            board[m.fromRow][m.fromCol].position = savedFrom;
            board[m.toRow][m.toCol].position = savedTo;

            if (maximizing && score > bestMove.score) {
                bestMove = m;
                bestMove.score = score;
            }

            if (!maximizing && score < bestMove.score) {
                bestMove = m;
                bestMove.score = score;
            }
        }

        return bestMove;
    }
}

