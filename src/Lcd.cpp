#include "Lcd.h"

namespace LCD
{
    unsigned long lastBlinkTime = 0;
    bool blinkState = true;

    byte topLeft[8] = {
        0b10011,  // row 0
        0b10011,  // row 1
        0b11100,  // row 2
        0b11100,  // row 3
        0b10011,  // row 4
        0b10011,  // row 5
        0b11100,  // row 6
        0b11100   // row 7
    };
    byte topMiddleLeft[8] = {
        0b00110,  // row 0
        0b00110,  // row 1
        0b11001,  // row 2
        0b11001,  // row 3
        0b00110,  // row 4
        0b00110,  // row 5
        0b11001,  // row 6
        0b11001   // row 7
        };
    byte topMiddleRight[8] = {
        0b01100,  // row 0
        0b01100,  // row 1
        0b10011,  // row 2
        0b10011,  // row 3
        0b01100,  // row 4
        0b01100,  // row 5
        0b10011,  // row 6
        0b10011   // row 7
    };
    byte topRight[8] = {
        0b11100,  // row 0
        0b11100,  // row 1
        0b00100,  // row 2
        0b00100,  // row 3
        0b11100,  // row 4
        0b11100,  // row 5
        0b00100,  // row 6
        0b00100   // row 7
    };
    byte bottomLeft[8] = {
        0b10011,  // row 0
        0b10011,  // row 1
        0b11100,  // row 2
        0b11100,  // row 3
        0b10011,  // row 4
        0b10011,  // row 5
        0b11100,  // row 6
        0b11100   // row 7
    };
    byte bottomMiddleLeft[8] = {
        0b00110,  // row 0
        0b00110,  // row 1
        0b11001,  // row 2
        0b11001,  // row 3
        0b00110,  // row 4
        0b00110,  // row 5
        0b11001,  // row 6
        0b11001   // row 7
    };
    byte bottomMiddleRight[8] = {
        0b01100,  // row 0
        0b01100,  // row 1
        0b10011,  // row 2
        0b10011,  // row 3
        0b01100,  // row 4
        0b01100,  // row 5
        0b10011,  // row 6
        0b10011   // row 7
    };
    byte bottomRight[8] = {
        0b11100,  // row 0
        0b11100,  // row 1
        0b00100,  // row 2
        0b00100,  // row 3
        0b11100,  // row 4
        0b11100,  // row 5
        0b00100,  // row 6
        0b00100   // row 7
    };

    byte inverse[8] = {
        0b11100,  // row 0
        0b11100,  // row 1
        0b00100,  // row 2
        0b00100,  // row 3
        0b11100,  // row 4
        0b11100,  // row 5
        0b11100,  // row 6
        0b11100   // row 7
    };
    
    LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

    void initLcd(){
        lcd.begin(16, 2);
        lcd.createChar(0, topLeft);
        lcd.createChar(1, topMiddleLeft);
        lcd.createChar(2, topMiddleRight);
        lcd.createChar(3, topRight);
        lcd.createChar(4, bottomLeft);
        lcd.createChar(5, bottomMiddleLeft);
        lcd.createChar(6, bottomMiddleRight);
        lcd.createChar(7, bottomRight);
    }

    void blinkChessBoard(int col, int row){
        if(row >= 5)

        while(true){
            unsigned long now = millis();
            if(now - lastBlinkTime >= BLINK_TIME_MS){
                blinkState = !blinkState;
                lastBlinkTime = now;
            }

            lcd.setCursor(0, 0);
            lcd.write(byte(0));
            lcd.write(byte(1));
            lcd.write(byte(2));
            lcd.write(byte(3));
            lcd.setCursor(0, 1);
            lcd.write(byte(4));
            lcd.write(byte(5));
            lcd.write(byte(6));
            lcd.write(byte(7));
            if (!blinkState) {
                lcd.createChar(7, inverse);
            }else{
                lcd.createChar(7, bottomRight);
            }

            delay(40);
        }
    }

    void print(String text, int line){
        blinkChessBoard(1,1);

        /*
        if(line == 0){
            lcd.setCursor(0, 0);
        }
        else{
            lcd.setCursor(0, 1);
        }

        lcd.print(text);*/
    }

    void clear(){
        lcd.clear();
    }
}