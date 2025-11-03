#include <Arduino.h>
#include <LibRobus.h>

//-----------------------------
// MAIN CODE.
//-----------------------------
void setup() {
    BoardInit();
    Serial.begin(115200);
}

void loop() {
  Serial.println("TEST");
}