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

void armDownShake(){
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_LOW-SMALL_SHAKE_OFFSET);
    delay(500);
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_LOW-BIG_SHAKE_OFFSET);
    delay(500);
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_LOW-SMALL_SHAKE_OFFSET);
    delay(500);
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_LOW-BIG_SHAKE_OFFSET);
    delay(500);
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_LOW-SMALL_SHAKE_OFFSET);
    delay(500);
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_LOW-BIG_SHAKE_OFFSET);
    delay(500);
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_LOW-SMALL_SHAKE_OFFSET);
    delay(500);
    armReset();
    SERVO_SetAngle(ARM_ID,ANGLE_LOW-BIG_SHAKE_OFFSET);
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
    magnetOn();
    armDown();
    delay(MAG_DELAY);
    armUp();
    delay(1000);
}

//dépose une pièce
void place(){
    armDown();
    delay(1000);
    magnetOff();
    delay(MAG_DELAY);
    //armDownShake();
    armUp();
    delay(1000);
}