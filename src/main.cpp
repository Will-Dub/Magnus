#include <Arduino.h>
#include <LibRobus.h>
#include "Chess.h"
#include "Lcd.h"

void getUserInput();
void printNextMove();

char selectedFromColChar = 'A';
int selectedFromRow = 1;
char selectedToColChar = 'A';
int selectedToRow = 1;
int currentIndex = 0;

const int BLINK_TIME_MS = 500;
unsigned long lastBlinkTime = 0;
bool blinkState = true;

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

void printNextMove() {
    unsigned long now = millis();

    if (now - lastBlinkTime > BLINK_TIME_MS) {
        blinkState = !blinkState;
        lastBlinkTime = now;
    }

    String from = String(selectedFromColChar) + String(selectedFromRow);
    String to = String(selectedToColChar) + String(selectedToRow);
    String displayText = from + " a " + to;

    if (!blinkState) {
        switch (currentIndex) {
            case 0: displayText.setCharAt(0, ' '); break;
            case 1: displayText.setCharAt(1, ' '); break;
            case 2: displayText.setCharAt(5, ' '); break;
            case 3: displayText.setCharAt(6, ' '); break;
        }
    }

    LCD::print(displayText, 1);
}

void getUserInput(){
    LCD::print("Entrer votre mouvement: ");

    while(currentIndex <= 3){
        printNextMove();
        /*if(rightPressed){
            currentIndex++;
            return;
        }

        if(leftPressed){
            currentIndex--;
            return;
        }

        if(upPressed){
            switch(currentIndex){
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                default:
                    break;
            }
        }

        if(downPressed){
            switch(currentIndex){
                case 0:
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                default:
                    break;
            }
        }
        }*/

        delay(100);
    }

    int selectedFromCol = CHESS::colCharToIndex(selectedFromColChar);
    int selectedToCol = CHESS::colCharToIndex(selectedToColChar);

    CHESS::MovePieceResult result = CHESS::movePiece(selectedFromCol, selectedFromRow-1, selectedToCol, selectedToRow-1);

    if(!result.isSuccess){
        String errorMessage = CHESS::getErrorMessage(result.erreur);
        LCD::print(errorMessage, 1);
        delay(2000);
        getUserInput();
        return;
    }
}
