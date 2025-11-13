#include "Menu.h"

namespace MENU {
    char selectedFromColChar = 'A';
    int selectedFromRow = 1;
    char selectedToColChar = 'A';
    int selectedToRow = 1;
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

    MoveInput getUserMove(bool isPreviousMoveInvalid){
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
        int colIndex = 0;
        int rowIndex = 0;
        bool isCurrentlyOnCol = true;

        while(!isOkPressedFlag){
            // Change d'index
            if(isCurrentlyOnCol && currentHorizontalIndex % 2 == 1){
                // Change de col à row
                currentVerticalIndex = rowIndex;
                isCurrentlyOnCol = false;
            }else if(!isCurrentlyOnCol && currentHorizontalIndex % 2 == 0){
                // Change de row à col
                currentVerticalIndex = colIndex;
                isCurrentlyOnCol = true;
            }

            // Met à jour l'index
            if (currentHorizontalIndex % 2 == 0){
                colIndex = currentVerticalIndex % 8;
            }
            else{
                rowIndex = currentVerticalIndex % 8;
            }

            selectedFromColChar = 'A' + colIndex;
            selectedFromRow = rowIndex + 1;

            //blinkChar(String(selectedFromColChar) + String(selectedFromRow));
            String text = "["+String(selectedFromColChar)+"]" + "["+String(selectedFromRow)+"]";
            blinkWord(text, 1, 1, 4, 4);
            tick();
        }

        // Dialogue: À
        reset();
        LCD::print("A:");

        while(!isOkPressedFlag){
            // Change d'index
            if(isCurrentlyOnCol && currentHorizontalIndex % 2 == 1){
                // Change de col à row
                currentVerticalIndex = rowIndex;
                isCurrentlyOnCol = false;
            }else if(!isCurrentlyOnCol && currentHorizontalIndex % 2 == 0){
                // Change de row à col
                currentVerticalIndex = colIndex;
                isCurrentlyOnCol = true;
            }

            // Met à jour col ou row
            if (currentHorizontalIndex % 2 == 0){
                colIndex = currentVerticalIndex % 8;
            }
            else{
                rowIndex = currentVerticalIndex % 8;
            }

            selectedToColChar = 'A' + colIndex;
            selectedToRow = rowIndex + 1;
            //blinkChar(String(selectedToColChar) + String(selectedToRow));
            String text = "["+String(selectedFromColChar)+"]" + "["+String(selectedFromRow)+"]";
            blinkWord(text, 1, 1, 4, 4);
            tick();
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

        /*bool downButtonPressed = digitalRead(PIN_DOWN_BUTTON);

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
        }*/

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

        /*bool leftButtonPressed = digitalRead(PIN_LEFT_BUTTON);

        if(leftButtonPressed){
            if(startLeftButtonPress == 0){
                isLeftPressed = false;
                startLeftButtonPress = currentTime;
            }
            else if (currentTime - startLeftButtonPress >= BUTTON_PRESS_TIME_MS) {
                if(!isLeftPressed){
                    currentHorizontalIndex++;
                    isLeftPressed = true;
                }
            }
        }else{
            isLeftPressed = false;
            startLeftButtonPress = 0;
        }*/

        delay(30);
    }
}