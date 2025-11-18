#include "Menu.h"

namespace MENU {
    int currentHorizontalIndex = 0;
    int currentVerticalIndex = 0;
    bool isOkPressedFlag = false;
    bool isRightPressedFlag = false;
    bool isUpPressedFlag = false;

    const int BLINK_TIME_MS = 500;
    unsigned long lastBlinkTime = 0;
    bool blinkState = true;

    void init(){
        pinMode(PIN_OK_BUTTON, INPUT);
        pinMode(PIN_UP_BUTTON, INPUT);
        pinMode(PIN_RIGHT_BUTTON, INPUT);
    }

    void waitForGameStart() {
        // Dialogue: Commencer
        reset();
        LCD::print("Commencer");

        while (!isOkPressedFlag) {
            blinkLine("      [OK]      ");
            tick();
        }
    }

    void waitForLoseAck(){
        // Dialogue: Perdu
        reset();
        LCD::print("Perdu :(");

        while (!isOkPressedFlag) {
            blinkLine("      [OK]      ");
            tick();
        }
    }

    MoveInput getUserMove(bool isPreviousMoveInvalid, bool isWhitePlaying){
        // Dialogue: Début de tour
        reset();
        if(!isPreviousMoveInvalid){
            LCD::print("A votre tour");   
        }else{
            LCD::print("Coup invalide");
        }

        while (!isOkPressedFlag) {
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
        reset();
        LCD::print("De:");
        int fromColIndex = 0;
        int fromRowIndex = 0;
        int oldColIndex = -1;
        int oldRowIndex = -1;

        if(!isWhitePlaying){
            currentHorizontalIndex = 1;
            currentVerticalIndex = 1;
        }

        while(!isOkPressedFlag){
            // Met à jour l'index
            if(isWhitePlaying){
                fromColIndex = ((currentHorizontalIndex % 8) + 8) % 8;
                fromRowIndex = ((currentVerticalIndex % 8) + 8) % 8;
            }else{
                fromColIndex = ((-currentHorizontalIndex % 8) + 8) % 8;
                fromRowIndex = ((-currentVerticalIndex % 8) + 8) % 8;
            }

            // Met à jour le chess board
            if(oldColIndex != fromColIndex || oldRowIndex != fromRowIndex){
                LCD::blinkChessBoard(fromColIndex, fromRowIndex, !isWhitePlaying);
                oldColIndex = fromColIndex;
                oldRowIndex = fromRowIndex;
            }

            // Affiche le mouvement
            char selectedFromColChar = 'A' + fromColIndex;
            int selectedFromRow = fromRowIndex + 1;

            String text = "["+String(selectedFromColChar)+"]" + "["+String(selectedFromRow)+"]";
            LCD::print(text,1);

            // Update le lcd et les boutons
            tick();
            LCD::tickBlinkChessBoard();
        }

        // Dialogue: À
        reset();
        LCD::print("A:");
        int toColIndex = 0;
        int toRowIndex = 0;
        oldColIndex = -1;
        oldRowIndex = -1;
        
        if(!isWhitePlaying){
            currentHorizontalIndex = 1;
            currentVerticalIndex = 1;
        }

        while(!isOkPressedFlag){
            // Met à jour col ou row
            if(isWhitePlaying){
                toColIndex = ((currentHorizontalIndex % 8) + 8) % 8;
                toRowIndex = ((currentVerticalIndex % 8) + 8) % 8;
            }else{
                toColIndex = ((-currentHorizontalIndex % 8) + 8) % 8;
                toRowIndex = ((-currentVerticalIndex % 8) + 8) % 8;
            }

            // Met à jour le chess board
            if(oldColIndex != toColIndex || oldRowIndex != toRowIndex){
                LCD::blinkChessBoard(toColIndex, toRowIndex, !isWhitePlaying);
                oldColIndex = toColIndex;
                oldRowIndex = toRowIndex;
            }

            // Affiche le mouvement
            char selectedToColChar = 'A' + toColIndex;
            int selectedToRow = toRowIndex + 1;

            String text = "["+String(selectedToColChar)+"]" + "["+String(selectedToRow)+"]";
            LCD::print(text,1);

            // Update le lcd et les boutons
            tick();
            LCD::tickBlinkChessBoard();
        }

        // Dialogue: Confirmation
        reset();
        LCD::print("Confirmer?");

        while(!isOkPressedFlag){
            blinkWord("[OUI]      [NON]", 0, 4, 11, 15);
            tick();
        }

        if(currentHorizontalIndex % 2 == 1){
            // Utilisateur pas sûr
            return getUserMove(isPreviousMoveInvalid, isWhitePlaying);
        }

        MENU::MoveInput input;
        input.fromColIndex = fromColIndex;
        input.fromRowIndex = fromRowIndex;
        input.toColIndex = toColIndex;
        input.toRowIndex = toRowIndex;
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

    void resetBlink(){
        blinkState = true;
        lastBlinkTime = millis();
    }

    void blinkLine(String text) {
        unsigned long now = millis();

        if (now - lastBlinkTime > BLINK_TIME_MS) {
            blinkState = !blinkState;
            lastBlinkTime = now;
        }

        if (!blinkState) {
            LCD::print("                ", 1);
            return;
        }

        LCD::print(text, 1);
    }

    void reset(){
        currentHorizontalIndex = 0;
        currentVerticalIndex = 0;
        isOkPressedFlag = false;
        LCD::clear();
    }

    void tick(){
        static bool isOkPressed = false;
        static unsigned long startOkButtonPress = 0;
        static bool isUpPressed = false;
        static unsigned long startUpButtonPress = 0;
        static bool isDownPressed = false;
        static unsigned long startDownButtonPress = 0;
        static bool isRightPressed = false;
        static unsigned long startRightButtonPress = 0;
        static bool isLeftPressed = false;
        static unsigned long startLeftButtonPress = 0;
        unsigned long currentTime = millis();

        bool okButtonPressed = digitalRead(PIN_OK_BUTTON);

        if(okButtonPressed){
            if(startOkButtonPress == 0){
                isOkPressed = false;
                startOkButtonPress = currentTime;
            }
            else if (currentTime - startOkButtonPress >= BUTTON_PRESS_TIME_MS) {
                if(!isOkPressed){
                    isOkPressedFlag = true;
                    isOkPressed = true;
                }
            }
        }else{
            isOkPressedFlag = false;
            isOkPressed = false;
            startOkButtonPress = 0;
        }

        bool upButtonPressed = digitalRead(PIN_UP_BUTTON);

        if(upButtonPressed){
            if(startUpButtonPress == 0){
                isUpPressed = false;
                startUpButtonPress = currentTime;
            }
            else if (currentTime - startUpButtonPress >= BUTTON_PRESS_TIME_MS) {
                if(!isUpPressed){
                    currentVerticalIndex++;
                    isUpPressed = true;
                    resetBlink();
                }
            }
        }else{
            isUpPressed = false;
            startUpButtonPress = 0;
        }

        bool downButtonPressed = digitalRead(PIN_DOWN_BUTTON);

        if(downButtonPressed){
            if(startDownButtonPress == 0){
                isDownPressed = false;
                startDownButtonPress = currentTime;
            }
            else if (currentTime - startDownButtonPress >= BUTTON_PRESS_TIME_MS) {
                if(!isDownPressed){
                    currentVerticalIndex--;
                    isDownPressed = true;
                }
            }
        }else{
            isDownPressed = false;
            startDownButtonPress = 0;
        }

        bool rightButtonPressed = digitalRead(PIN_RIGHT_BUTTON);

        if(rightButtonPressed){
            if(startRightButtonPress == 0){
                isRightPressed = false;
                startRightButtonPress = currentTime;
            }
            else if (currentTime - startRightButtonPress >= BUTTON_PRESS_TIME_MS) {
                if(!isRightPressed){
                    currentHorizontalIndex++;
                    isRightPressed = true;
                    resetBlink();
                }
            }
        }else{
            isRightPressed = false;
            startRightButtonPress = 0;
        }

        bool leftButtonPressed = digitalRead(PIN_LEFT_BUTTON);

        if(leftButtonPressed){
            if(startLeftButtonPress == 0){
                isLeftPressed = false;
                startLeftButtonPress = currentTime;
            }
            else if (currentTime - startLeftButtonPress >= BUTTON_PRESS_TIME_MS) {
                if(!isLeftPressed){
                    currentHorizontalIndex--;
                    isLeftPressed = true;
                }
            }
        }else{
            isLeftPressed = false;
            startLeftButtonPress = 0;
        }

        delay(30);
    }
}