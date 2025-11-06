#include <Arduino.h>
#include <LibRobus.h>
#include "Chess.h"

void getUserInput();

//-----------------------------
// MAIN CODE.
//-----------------------------
void setup() {
    //BoardInit();
    Serial.begin(115200);
    CHESS::setupBoard();
}

void loop() {
    CHESS::printBoard();
    getUserInput();
}

char readCharFromSerial() {
    while (Serial.available() == 0) {}
    return Serial.read();
}

// A-H -> 0-7
int colCharToIndex(char c) {
  if (c >= 'A' && c <= 'H') return c - 'A';
  if (c >= 'a' && c <= 'h') return c - 'a';
  return -1;
}

// 1-8 -> 0-7
int rowCharToIndex(char c) {
  if (c >= '1' && c <= '8') return c - '1';
  return -1;
}

void getUserInput(){
    CHESS::printCurrentPlayer();
    Serial.println("Entrer votre mouvement:");
    
    String input = "";
    while (input.length() < 5) {
        if (Serial.available() > 0) {
            char c = Serial.read();
            if (c != '\n' && c != '\r') input += c;
        }
    }

    int fromCol = colCharToIndex(input[0]);
    int fromRow = rowCharToIndex(input[1]);
    int toCol   = colCharToIndex(input[3]);
    int toRow   = rowCharToIndex(input[4]);

    // Check
    if (fromCol == -1 || fromRow == -1 || toCol == -1 || toRow == -1) {
        Serial.println("Entrée invalide, veuillez réessayer.");
        getUserInput();
        return;
    }

    CHESS::MovePieceResult result = CHESS::movePiece(fromCol, fromRow, toCol, toRow);

    if(!result.isSuccess){
        CHESS::printCurrentPlayer();
        CHESS::printError(result.erreur);
        getUserInput();
        return;
    }
}
