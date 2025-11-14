#include "Lcd.h"

namespace LCD
{
    unsigned long lastBlinkTime = 0;
    bool blinkState = true;

    byte topLeft[8] = {
        0b00100,  // row 0
        0b00100,  // row 1
        0b00111,  // row 2
        0b00111,  // row 3
        0b00100,  // row 4
        0b00100,  // row 5
        0b00111,  // row 6
        0b00111   // row 7
    };
    byte topMiddleLeft[8] = {
        0b11001,  // row 0
        0b11001,  // row 1
        0b00110,  // row 2
        0b00110,  // row 3
        0b11001,  // row 4
        0b11001,  // row 5
        0b00110,  // row 6
        0b00110   // row 7
        };
    byte topMiddleRight[8] = {
        0b10011,  // row 0
        0b10011,  // row 1
        0b01100,  // row 2
        0b01100,  // row 3
        0b10011,  // row 4
        0b10011,  // row 5
        0b01100,  // row 6
        0b01100   // row 7
    };
    byte topRight[8] = {
        0b00111,  // row 0
        0b00111,  // row 1
        0b11001,  // row 2
        0b11001,  // row 3
        0b00111,  // row 4
        0b00111,  // row 5
        0b11001,  // row 6
        0b11001   // row 7
    };
    byte bottomLeft[8] = {
        0b00100,  // row 0
        0b00100,  // row 1
        0b00111,  // row 2
        0b00111,  // row 3
        0b00100,  // row 4
        0b00100,  // row 5
        0b00111,  // row 6
        0b00111   // row 7
    };
    byte bottomMiddleLeft[8] = {
        0b11001,  // row 0
        0b11001,  // row 1
        0b00110,  // row 2
        0b00110,  // row 3
        0b11001,  // row 4
        0b11001,  // row 5
        0b00110,  // row 6
        0b00110   // row 7
    };
    byte bottomMiddleRight[8] = {
        0b10011,  // row 0
        0b10011,  // row 1
        0b01100,  // row 2
        0b01100,  // row 3
        0b10011,  // row 4
        0b10011,  // row 5
        0b01100,  // row 6
        0b01100   // row 7
    };
    byte bottomRight[8] = {
        0b00111,  // row 0
        0b00111,  // row 1
        0b11001,  // row 2
        0b11001,  // row 3
        0b00111,  // row 4
        0b00111,  // row 5
        0b11001,  // row 6
        0b11001   // row 7
    };

    byte initial1[8] = {};

    byte inverse1[8] = {};

    byte initial2[8] = {};

    byte inverse2[8] = {};

    int indexToChangePixel1;
    int indexToChangePixel2;
    
    LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

    void initLcd(){
        lcd.begin(16, 2);
    }

    /**
     * @brief Copy le charactère de la position qui doit être inversé
     * 
     * @param mapX position x de la map (0-3) 
     * @param mapY position y de la map (0-1)
     * @param inverse pointeur vers la map destinataire
     */
    void copyMap(int mapX, int mapY, byte inverse[8]) {
        byte* selected;
        switch(mapX) {
            case 0:
                // Left
                selected = (mapY ? topLeft : bottomLeft);
                break;

            case 1:
                // Middle left
                selected = (mapY ? topMiddleLeft : bottomMiddleLeft);
                break;

            case 2:
                // Middle right
                selected = (mapY ? topMiddleRight : bottomMiddleRight);
                break;

            case 3:
                // Right
                selected = (mapY ? topRight : bottomRight);
                break;

            default:
                selected = topLeft;
                break;
        }

        for (int i = 0; i < 8; i++) {
            inverse[i] = selected[i];
        }
    }

    /**
     * @brief inverse une demi case
     * 
     * @param x position x du pixel (0-4)
     * @param y position y du pixel du haut (0-3)
     * @param inverse map à inverser
     */
    void inverseHalfSquare(int x, int y, byte inverse[8]){
        int yPos = y*2;
        inverse[yPos] ^= (1 << x);
        inverse[yPos+1] ^= (1 << x);
    }

    void blinkChessBoard(int col, int row){
        lcd.createChar(0, topLeft);
        lcd.createChar(1, topMiddleLeft);
        lcd.createChar(2, topMiddleRight);
        lcd.createChar(3, topRight);
        lcd.createChar(4, bottomLeft);
        lcd.createChar(5, bottomMiddleLeft);
        lcd.createChar(6, bottomMiddleRight);
        lcd.createChar(7, bottomRight);

        // Col entre 0-7, row entre 0-7
        // 0 = left, 1 = middle left, 2 = middle right, 3 = right 
        int mapXPixel1 = mapXPixel1Table[col];
        int mapXPixel2 = mapXPixel2Table[col];
        int mapY = mapYTable[row];
        indexToChangePixel1 = mapPosToIndex[mapY][mapXPixel1];
        indexToChangePixel2 = mapPosToIndex[mapY][mapXPixel2];
        
        copyMap(mapXPixel1, mapY, initial1);
        copyMap(mapXPixel1, mapY, inverse1);

        if(mapXPixel1 != mapXPixel2){
            copyMap(mapXPixel2, mapY, inverse2);
            copyMap(mapXPixel2, mapY, initial2);
        }

        // inverse la case
        if(mapXPixel1 != mapXPixel2){
            inverseHalfSquare(mapXToPixel1Pos[col], mapYToPixelPos[row], inverse1);
            inverseHalfSquare(mapXToPixel2Pos[col], mapYToPixelPos[row], inverse2);
        }else{
            inverseHalfSquare(mapXToPixel1Pos[col], mapYToPixelPos[row], inverse1);
            inverseHalfSquare(mapXToPixel2Pos[col], mapYToPixelPos[row], inverse1);
        }

        lcd.setCursor(12, 0);
        lcd.write(byte(0));
        lcd.write(byte(1));
        lcd.write(byte(2));
        lcd.write(byte(3));
        lcd.setCursor(12, 1);
        lcd.write(byte(4));
        lcd.write(byte(5));
        lcd.write(byte(6));
        lcd.write(byte(7));
    }

    void tickBlinkChessBoard(){
        unsigned long now = millis();
        if(now - lastBlinkTime >= BLINK_TIME_MS){
            blinkState = !blinkState;
            lastBlinkTime = now;
        }

        if (!blinkState) {
            lcd.createChar(indexToChangePixel1, inverse1);

            if(indexToChangePixel1 != indexToChangePixel2){
                lcd.createChar(indexToChangePixel2, inverse2);
            }
        }else{
            lcd.createChar(indexToChangePixel1, initial1);

            if(indexToChangePixel1 != indexToChangePixel2){
                lcd.createChar(indexToChangePixel2, initial2);
            }
        }
    }

    void print(String text, int line){
        if(line == 0){
            lcd.setCursor(0, 0);
        }
        else{
            lcd.setCursor(0, 1);
        }

        lcd.print(text);
    }

    void clear(){
        lcd.clear();
    }
}