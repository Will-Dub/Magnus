#include <Arduino.h>
#include <LibRobus.h>
#include "ArmMagnetControl.h"

//initialise l'aimant
void magSetup(){
    pinMode(MAGNET_PIN,OUTPUT);
}

//allume/initialise le bras (servo moteur)
void armOn(){
    SERVO_Enable(ARM_ID);
}

//éteint/désactive le bras (servo moteur)
void armOff(){
    SERVO_Disable(ARM_ID);
}

//réinitialise le bras (servo moteur)
void armReset(){
    armOff();
    armOn();
}

//met le bras en position basse
void armDown(){
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_LOW);
}

//met le bras en position haute
void armUp(){
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_HIGH);
}

//=============partie aimant==========

//allume l'aimant
void magnetOn(){
    digitalWrite(MAGNET_PIN,HIGH);
}

//éteint l'aimant
void magnetOff(){
    digitalWrite(MAGNET_PIN,LOW);
}

//=============partie ramasse=========

//ramasse une pièce
void pickup(){
    armDown();
    magnetOn();
    delay(MAG_DELAY);
    armUp();
}

//dépose une pièce
void place(){
    armDown();
    magnetOff();
    delay(MAG_DELAY);
    armUp();
}