#include <Arduino.h>
#include <LibRobus.h>
#include "Chess.h"
#include "Lcd.h"
#include "Menu.h"
#include "ChessMovement.h"
#include "ArmMagnetControl.h"

bool isGameStarted = false;
bool isPreviousMoveInvalid = false;

//-----------------------------
// MAIN CODE.
//-----------------------------
void setup() {
    BoardInit();
    Serial.begin(115200);
    MOVEMENT::init();
    CHESS::setupBoard();
    MENU::init();
    LCD::initLcd();
    magSetup();
}

void loop() {
    if (!isGameStarted) {
        MENU::waitForGameStart();
        isGameStarted = true;
    }

    // Print the board on serial
    CHESS::printBoard();
    CHESS::printCurrentPlayer();

    // Get the user move
    bool isWhitePlaying = CHESS::getCurrentTurn() == CHESS::Player::WHITE;
    MENU::MoveInput moveInput = MENU::getUserMove(isPreviousMoveInvalid, isWhitePlaying);

    // Execute the move
    if(moveInput.giveUp){
        isGameStarted = false;
        isPreviousMoveInvalid = false;
        MENU::waitForLoseAck();

        if(!isWhitePlaying){
            CHESS_MOVEMENT::moveFromBlackToWhite();
        }

        return;
    }

    CHESS::MovePieceResult result = CHESS::movePiece(moveInput.fromColIndex, moveInput.fromRowIndex, moveInput.toColIndex, moveInput.toRowIndex);

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
    delay(1000);
    // Capture move
    /*if (result.isPawnOnDest) 
    {
        // Collect the pice and drop it off
        if (isWhitePlaying)
            CHESS_MOVEMENT::moveFromWhiteToSquare(moveInput.toColIndex, moveInput.toRowIndex);
        else
            CHESS_MOVEMENT::moveFromBlackToSquare(moveInput.toColIndex, moveInput.toRowIndex);

        CHESS_MOVEMENT::moveFromSquareToDropOff(moveInput.toColIndex, moveInput.toRowIndex);

        // Move to the from square
        CHESS_MOVEMENT::moveFromDropOffToSquare(moveInput.fromColIndex, moveInput.fromRowIndex);

        // Move to the to square
        CHESS_MOVEMENT::moveSquareToSquare(
            moveInput.fromColIndex, moveInput.fromRowIndex,
            moveInput.toColIndex,   moveInput.toRowIndex
        );

        // Return
        if (isWhitePlaying)
            CHESS_MOVEMENT::moveFromSquareToBlack(moveInput.toColIndex, moveInput.toRowIndex);
        else
            CHESS_MOVEMENT::moveFromSquareToWhite(moveInput.toColIndex, moveInput.toRowIndex);
    }
    // Normal move
    else 
    {
        if (isWhitePlaying)
            CHESS_MOVEMENT::moveFromWhiteToSquare(moveInput.fromColIndex, moveInput.fromRowIndex);
        else
            CHESS_MOVEMENT::moveFromBlackToSquare(moveInput.fromColIndex, moveInput.fromRowIndex);

        CHESS_MOVEMENT::moveSquareToSquare(
            moveInput.fromColIndex, moveInput.fromRowIndex,
            moveInput.toColIndex,   moveInput.toRowIndex
        );

        if (isWhitePlaying)
            CHESS_MOVEMENT::moveFromSquareToBlack(moveInput.toColIndex, moveInput.toRowIndex);
        else
            CHESS_MOVEMENT::moveFromSquareToWhite(moveInput.toColIndex, moveInput.toRowIndex);
    }*/
    
    isPreviousMoveInvalid = false;
}
