#include <Arduino.h>
#include <LibRobus.h>
#include "Chess.h"
#include "Lcd.h"
#include "Menu.h"
#include "ChessMovement.h"

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
    CHESS_MOVEMENT::moveFromOriginTo(1,1);
    delay(100000);
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

    int selectedFromCol = CHESS::colCharToIndex(moveInput.fromCol);
    int selectedToCol = CHESS::colCharToIndex(moveInput.toCol);

    CHESS::MovePieceResult result = CHESS::movePiece(selectedFromCol, moveInput.fromRow-1, selectedToCol, moveInput.toRow-1);

    if(!result.isSuccess){
        String errorMessage = CHESS::getErrorMessage(result.erreur);
        Serial.println(errorMessage);
        isPreviousMoveInvalid = true;
        return;
    }

    isPreviousMoveInvalid = false;
}
