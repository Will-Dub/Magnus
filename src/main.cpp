#include <Arduino.h>
#include <LibRobus.h>
#include "Chess.h"
#include "Lcd.h"
#include "Menu.h"
#include "ChessMovement.h"
#include "ArmMagnetControl.h"

bool isGameStarted = false;
bool isGameVSAi = false;
bool isPreviousMoveInvalid = false;

CHESS::MovePieceResult getPlayerMove();
CHESS::MovePieceResult getRobotMove();
void executeMoveOnBoard(CHESS::MovePieceResult move);

//-----------------------------
// MAIN CODE.
//-----------------------------
void setup() {
    BoardInit();
    Serial.begin(115200);
    magSetup();
    MOVEMENT::init();
    CHESS::setupBoard();
    MENU::init();
    LCD::initLcd();
    magSetup();
    armUp();

    /*while(true){
        MOVEMENT::moveForwardNonBlocking(999999);
        CHESS_MOVEMENT::waitEndMoveByLineNb(1);
        delay(1000);
    }*/

    /*CHESS_MOVEMENT::turnLeft();
    
    while(true){
        char value = LINE::ucReadLineSensors();
        uint8_t bits = value & 0b00000111;

        // Convert bits to string
        String bitString;
        for (int i = 2; i >= 0; --i) {        // from MSB to LSB of the 3 bits
            bitString += ((bits >> i) & 1) ? '1' : '0';
        }

        LCD::print(bitString);
        delay(100);
    }*/
}

void loop() {
    if (!isGameStarted) {
        isGameVSAi = true;
        isGameStarted = true;
    }

    // Print the board on serial
    CHESS::printBoard();
    CHESS::printCurrentPlayer();

    CHESS::MovePieceResult result;
    result = getRobotMove();

    // Vérifie si la partie est perdu
    if(result.isGameLost){
        isGameStarted = false;
        isPreviousMoveInvalid = false;
        LCD::clear();
        LCD::print("PERDU");
        delay(1000000);

        return;
    }

    // Check if the move is invalid
    if(!result.isSuccess){
        String errorMessage = CHESS::getErrorMessage(result.erreur);
        Serial.println(errorMessage);
        isPreviousMoveInvalid = true;
        return;
    }

    // Execute the move physically
    LCD::clear();
    LCD::print("EN COURS...");
    executeMoveOnBoard(result);
    
    isPreviousMoveInvalid = false;
}

CHESS::MovePieceResult getPlayerMove(){
    // Get the user move
    bool isWhitePlaying = CHESS::getCurrentTurn() == CHESS::Player::WHITE;
    MENU::MoveInput moveInput = MENU::getUserMove(isPreviousMoveInvalid, isWhitePlaying);

    // Execute the move
    if(moveInput.giveUp){
        MENU::waitForLoseAck();

        if(!isWhitePlaying){
            CHESS_MOVEMENT::moveFromBlackToWhite();
        }

        CHESS::MovePieceResult result;
        result.isGameLost = true;

        return result;
    }

    return CHESS::movePiece(moveInput.fromColIndex, moveInput.fromRowIndex, moveInput.toColIndex, moveInput.toRowIndex);
}

CHESS::MovePieceResult getRobotMove(){
    CHESS::MinimaxMove minimaxMove = CHESS::findBestMove(2);

    CHESS::MovePieceResult result;
    result = CHESS::movePiece(minimaxMove.fromCol, minimaxMove.fromRow, minimaxMove.toCol, minimaxMove.toRow);
    
    result.isGameLost = !result.isSuccess;

    return result;
}

void executeMoveOnBoard(CHESS::MovePieceResult move){
    bool isWhitePlaying = move.player == CHESS::Player::WHITE;

    // Capture move
    if (move.isPawnOnDest) 
    {
        // Collect the pice and drop it off
        if (isWhitePlaying)
            CHESS_MOVEMENT::moveFromWhiteToSquare(move.toCol, move.toRow);
        else
            CHESS_MOVEMENT::moveFromBlackToSquare(move.toCol, move.toRow);
        pickup();

        CHESS_MOVEMENT::moveFromSquareToDropOff(move.toCol, move.toRow);
        place();

        // Move to the from square
        CHESS_MOVEMENT::moveFromDropOffToSquare(move.fromCol, move.fromRow);
        pickup();

        // Move to the to square
        CHESS_MOVEMENT::moveSquareToSquare(
            move.fromCol, move.fromRow,
            move.toCol, move.toRow
        );
        place();

        // Return
        if (isWhitePlaying)
            CHESS_MOVEMENT::moveFromSquareToBlack(move.toCol, move.toRow);
        else
            CHESS_MOVEMENT::moveFromSquareToWhite(move.toCol, move.toRow);
    }
    // Normal move
    else 
    {
        if (isWhitePlaying)
            CHESS_MOVEMENT::moveFromWhiteToSquare(move.fromCol, move.fromRow);
        else
            CHESS_MOVEMENT::moveFromBlackToSquare(move.fromCol, move.fromRow);
        pickup();

        CHESS_MOVEMENT::moveSquareToSquare(
            move.fromCol, move.fromRow,
            move.toCol,   move.toRow
        );
        place();

        if (isWhitePlaying)
            CHESS_MOVEMENT::moveFromSquareToBlack(move.toCol, move.toRow);
        else
            CHESS_MOVEMENT::moveFromSquareToWhite(move.toCol, move.toRow);
    }
}
