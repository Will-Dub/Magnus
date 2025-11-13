#include "Menu.h"

namespace MENU {
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

    bool waitForGameStart() {
        // Dialogue: Commencer
        resetMenuIndex();
        LCD::print("Commencer");
        LCD::print("      [OK]      ", 1);

        while (!isOkPressed) {
            tick();
        }

        // Dialogue: Choix couleur
        resetMenuIndex();
        LCD::print("Choix couleur");

        while (!isOkPressed) {
            blinkWord("[BLANC]   [NOIR]", 0, 6, 10, 15);
            tick();
        }

        bool isPlayerWhite = currentHorizontalIndex % 2;
        return isPlayerWhite;
    }

    MoveInput getUserMove(bool isPreviousMoveInvalid){
        // Dialogue: Début de tour
        resetMenuIndex();
        if(!isPreviousMoveInvalid){
            LCD::print("A votre tour");   
        }else{
            LCD::print("Coup invalide");
        }

        while (!isOkPressed) {
            blinkWord("[OK]  [Renoncer]", 0, 3, 6, 15);
            tick();
        }

        if(currentHorizontalIndex % 2 == 1){
            // Joueur renonce
            MENU::MoveInput input;
            input.giveUp = true;
            return input;
        }

        // Dialogue: De
        resetMenuIndex();
        LCD::print("De:");
        int colIndex = 0;
        int rowIndex = 0;

        while(!isOkPressed){
            // Met à jour col ou row
            if (currentHorizontalIndex % 2 == 0){
                colIndex = currentVerticalIndex % 8;
            }
            else{
                rowIndex = currentVerticalIndex % 8;
            }

            selectedFromColChar = 'A' + colIndex;
            selectedFromRow = rowIndex + 1;

            blinkChar(String(selectedFromColChar) + String(selectedFromRow));
            tick();
        }

        // Dialogue: À
        resetMenuIndex();
        LCD::print("A:");

        while(!isOkPressed){
            // Met à jour col ou row
            if (currentHorizontalIndex % 2 == 0){
                colIndex = currentVerticalIndex % 8;
            }
            else{
                rowIndex = currentVerticalIndex % 8;
            }

            selectedToColChar = 'A' + colIndex;
            selectedToRow = rowIndex + 1;
            blinkChar(String(selectedToColChar) + String(selectedToRow));
            tick();
        }

        // Dialogue: Confirmation
        resetMenuIndex();
        LCD::print("Confirmer?");

        while(!isOkPressed){
            blinkWord("[OUI]      [NON]", 0, 4, 11, 15);
            tick();
        }

        if(currentHorizontalIndex % 2 == 1){
            // Utilisateur pas sûr
            return getUserMove(isPreviousMoveInvalid);
        }

        MENU::MoveInput input;
        input.fromCol = selectedFromColChar;
        input.fromRow = selectedFromRow;
        input.toCol = selectedToColChar;
        input.toRow = selectedToRow;
        input.giveUp = false;
        return input;
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

    void tick(){
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
}