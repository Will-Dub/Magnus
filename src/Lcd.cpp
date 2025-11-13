#include "Lcd.h"

namespace LCD
{
    LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

    void initLcd(){
        lcd.begin(16, 2);
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