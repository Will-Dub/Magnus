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
    armOn();
}

void loop() {
    if (!isGameStarted) {
        MENU::waitForGameStart();
        isGameStarted = true;
    }

    CHESS::printBoard();
    CHESS::printCurrentPlayer();
    MENU::MoveInput moveInput = MENU::getUserMove(isPreviousMoveInvalid);

    if(moveInput.giveUp){
        isGameStarted = false;
        MENU::waitForLoseAck();
        return;
    }

    CHESS::MovePieceResult result = CHESS::movePiece(moveInput.fromColIndex, moveInput.fromRowIndex, moveInput.toColIndex, moveInput.toRowIndex);

    if(!result.isSuccess){
        String errorMessage = CHESS::getErrorMessage(result.erreur);
        Serial.println(errorMessage);
        isPreviousMoveInvalid = true;
        return;
    }

    // Execute the move with the robot
    if(result.isPawnOnDest){
        //CHESS_MOVEMENT::moveFromWhiteToSquare(selectedToCol)
    }

    isPreviousMoveInvalid = false;
}
