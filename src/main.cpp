#include <Arduino.h>
#include <LibRobus.h>
#include "Chess.h"
#include "Lcd.h"

void getUserInput();
void waitForGameStart();
void menuControlTick();
void resetMenuIndex();
void blinkWord(String text, unsigned int startWord1, unsigned int endWord1, unsigned int startWord2, unsigned int endWord2);
void blinkChar(String text, int optionCount = 2);

char selectedFromColChar = 'A';
int selectedFromRow = 1;
char selectedToColChar = 'A';
int selectedToRow = 1;
int currentHorizontalIndex = 0;
int currentVerticalIndex = 0;
bool isOkPressed = false;

const int BLINK_TIME_MS = 500;
unsigned long lastBlinkTime = 0;
bool blinkState = true;
bool isGameStarted = false;

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
    if(!isGameStarted){
        waitForGameStart();
    }

    CHESS::printBoard();
    CHESS::printCurrentPlayer();
    getUserInput();
}

void waitForGameStart() {
    resetMenuIndex();
    LCD::print("Commencer");
    LCD::print("      [OK]      ", 1);

    while (!isOkPressed) {
        menuControlTick();
    }

    resetMenuIndex();
    LCD::print("Choix couleur");

    while (!isOkPressed) {
        blinkWord("[BLANC]   [NOIR]", 0, 6, 10, 15);
        menuControlTick();
    }

    isGameStarted = true;
}

void blinkChar(String text, int optionCount) {
    unsigned long now = millis();
    unsigned int index = currentHorizontalIndex % optionCount;

    if (now - lastBlinkTime > BLINK_TIME_MS) {
        blinkState = !blinkState;
        lastBlinkTime = now;
    }

    if (!blinkState && index < text.length()) {
        text.setCharAt(index, ' ');
    }

    LCD::print(text, 1);
}

void blinkWord(String text, unsigned int startWord1, unsigned int endWord1, unsigned int startWord2, unsigned int endWord2) {
    unsigned long now = millis();
    unsigned int index = currentHorizontalIndex % 2;

    if (now - lastBlinkTime > BLINK_TIME_MS) {
        blinkState = !blinkState;
        lastBlinkTime = now;
    }

    if (!blinkState) {
        unsigned int startWord = index == 0 ? startWord1 : startWord2;
        unsigned int endWord = index == 0 ? endWord1 : endWord2;
        
        if (startWord >= 0 && endWord < text.length()) {
            for (unsigned int i = startWord; i <= endWord; ++i) {
                text[i] = ' ';
            }
        }
    }

    LCD::print(text, 1);
}

void resetMenuIndex(){
    currentHorizontalIndex = 0;
    currentVerticalIndex = 0;
    isOkPressed = false;
}

void menuControlTick(){
    /*if(isRightPressed){
        currentHorizontalIndex++;
        return;
    }

    if(isLeftPressed){
        currentHorizontalIndex--;
        return;
    }

    if(isUpPressed){
        currentVerticalIndex++;
    }

    if(isDownPressed){
        currentVerticalIndex--;
    }
        
    if(isOkPressed){
        isOkPressed = true
    }*/

    delay(100);
}

void getUserInput(){
    // TODO: Ajout choix utilisateur
    resetMenuIndex();
    LCD::print("A votre tour");

    while (!isOkPressed) {
        blinkWord("[OK]  [Renoncer]", 0, 3, 6, 15);
        menuControlTick();
    }

    resetMenuIndex();
    LCD::print("De:");

    while(!isOkPressed){
        //TODO: Change selected col + row
        blinkChar(String(selectedFromColChar) + String(selectedFromRow));
        menuControlTick();
    }

    resetMenuIndex();
    LCD::print("A:");

    while(!isOkPressed){
        //TODO: Change selected col + row
        blinkChar(String(selectedToColChar) + String(selectedToRow));
        menuControlTick();
    }

    resetMenuIndex();
    LCD::print("Êtes-vous sûr");

    while(!isOkPressed){
        blinkWord("[OUI]      [NON]", 0, 4, 11, 15);
        menuControlTick();
    }

    int selectedFromCol = CHESS::colCharToIndex(selectedFromColChar);
    int selectedToCol = CHESS::colCharToIndex(selectedToColChar);

    CHESS::MovePieceResult result = CHESS::movePiece(selectedFromCol, selectedFromRow-1, selectedToCol, selectedToRow-1);

    if(!result.isSuccess){
        String errorMessage = CHESS::getErrorMessage(result.erreur);
        Serial.println(errorMessage);

        resetMenuIndex();
        LCD::print("Coup invalide");
        LCD::print("      [OK]      ", 1);

        while(!isOkPressed){
            menuControlTick();
        }

        getUserInput();
        return;
    }
}
