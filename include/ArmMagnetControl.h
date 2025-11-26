//temps de délai pour l'aimant
const int MAG_DELAY = 3000;
//numéro de la pin qui contrôle l'électroaimant
const int MAGNET_PIN = 37;
//numéro du id du bras (où il est connecté, 0 ou 1)
const int ARM_ID = 1;
//angles du servo moteur pour les positions
const int ANGLE_HIGH = 0;
const int ANGLE_LOW = 66;
const int SMALL_SHAKE_OFFSET = 0;
const int BIG_SHAKE_OFFSET = 10;

//permet l'appel des fonctions pour controller la position du bras et l'aimant

void armOn();
void armOff();
void armReset();
void armUp();
void armDown();
void armDownShake();

void magSetup(); 
void magnetOn();
void magnetOff();

void pickup();
void place();