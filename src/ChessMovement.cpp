#include "ChessMovement.h"

namespace CHESS_MOVEMENT{
    void moveFromOriginTo(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        // Ce rend SUR la ligne
        if(destRow != 0){
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destRow);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM);
        }

        // Tourne pour regarder l'EST
        MOVEMENT::turnRight(30);
        MOVEMENT::turnRightUntilLine();

        // Avance jusqu'au milieu de la case
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(destCol+1);

        // Tourne pour regarder le NORD
        MOVEMENT::turnLeft(90);

        // Avance jusqu'au milieu
        MOVEMENT::moveUntilLine();
    }

    void waitEndMoveByLineNb(int nbLine){
        int detectedLines = 0;
        bool oldWasOnAllLine = false;

        while(MOVEMENT::getCurrentMove() != MOVEMENT::MoveEnum::NONE){
            MOVEMENT::runMovementController();

            if(!oldWasOnAllLine && LINE::ucReadLineSensors() == 0b111){
                detectedLines++;
                oldWasOnAllLine = true;
            }else if(LINE::ucReadLineSensors() != 0b111){
                oldWasOnAllLine = false;
            }

            if(detectedLines >= nbLine){
                MOVEMENT::stop();
            }
            
            delay(2);
        }
    }
}