#include <Arduino.h>
#include <LibRobus.h>
#include "Chess.h"
#include "Lcd.h"

void getUserInput();

//-----------------------------
// MAIN CODE.
//-----------------------------
void setup() {
    //BoardInit();
    Serial.begin(115200);
    CHESS::setupBoard();
    LCD::initLcd();
}

void loop() {
    CHESS::printBoard();
    CHESS::printCurrentPlayer();
    getUserInput();
}

char readCharFromSerial() {
    while (Serial.available() == 0) {}
    return Serial.read();
}

void getUserInput(){
    Serial.print("Entrer votre mouvement: ");
    
    String input = "";
    while (input.length() < 5) {
        if (Serial.available() > 0) {
            char c = Serial.read();
            if (c == 'R' || c == 'r') {
                CHESS::setupBoard();
                return;
            }
            if (c != '\n' && c != '\r') input += c;
        }
    }

    int fromCol = CHESS::colCharToIndex(input[0]);
    int fromRow = CHESS::rowCharToIndex(input[1]);
    int toCol = CHESS::colCharToIndex(input[3]);
    int toRow = CHESS::rowCharToIndex(input[4]);

    // Check
    if (fromCol == -1 || fromRow == -1 || toCol == -1 || toRow == -1) {
        Serial.println("Entrée invalide, veuillez réessayer.");
        getUserInput();
        return;
    }

    CHESS::MovePieceResult result = CHESS::movePiece(fromCol, fromRow, toCol, toRow);

    if(!result.isSuccess){
        CHESS::printError(result.erreur);
        getUserInput();
        return;
    }
}
