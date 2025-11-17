#include <Arduino.h>
#include <LibRobus.h>
#include "ArmMagnetControl.h"

//numéro du id du bras (où il est connecté, 0 ou 1)
const int arm = 0;

//angles du servo moteur pour les positions
const int angleHigh = 180;
const int angleLow = 0;

//numéro de la pin qui contrôle l'électroaimant
const int magnet = 49;

//initialise l'aimant
void magSetup(){
    pinMode(magnet,OUTPUT);
}

//allume/initialise le bras (servo moteur)
void armOn(){
    SERVO_Enable(arm);
}

//éteint/désactive le bras (servo moteur)
void armOff(){
    SERVO_Disable(arm);
}

//réinitialise le bras (servo moteur)
void armReset(){
    armOff();
    armOn();
}

//met le bras en position basse
void armDown(){
    armReset();
    SERVO_SetAngle(arm,angleLow);
}

//met le bras en position haute
void armUp(){
    armReset();
    SERVO_SetAngle(arm,angleHigh);
}

//=============partie aimant==========

//allume l'aimant
void magnetOn(){
    digitalWrite(magnet,HIGH);
}

//éteint l'aimant
void magnetOff(){
    digitalWrite(magnet,LOW);
}

//=============partie ramasse=========

//ramasse une pièce
void pickup(){
    armDown();
    magnetOn();
    armUp();
}

//dépose une pièce
void place(){
    armDown();
    magnetOff();
    armUp();
}