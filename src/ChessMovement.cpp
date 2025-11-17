#include "ChessMovement.h"

namespace CHESS_MOVEMENT{
    void moveFromWhiteToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        if(destRow != 0){
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destRow);
        }

        if(destCol <= 3){
            MOVEMENT::turnLeft(45);
            MOVEMENT::turnLeftUntilLine();
            
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(4 - destCol);

            MOVEMENT::turnRight(85);
        }else if(destCol >= 4){
            MOVEMENT::turnRight(45);
            MOVEMENT::turnRightUntilLine();
            
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destCol - 3);

            MOVEMENT::turnLeft(85);
        }

        // Avance jusqu'au milieu
        MOVEMENT::moveUntilLine();
    }

    void moveFromBlackToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(8 - destRow);

        if(destCol <= 3){
            MOVEMENT::turnRight(45);
            MOVEMENT::turnRightUntilLine();
            
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(4 - destCol);

            MOVEMENT::turnRight(85);
        }else if(destCol >= 4){
            MOVEMENT::turnLeft(45);
            MOVEMENT::turnLeftUntilLine();
            
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destCol - 3);

            MOVEMENT::turnLeft(85);
        }

        // Avance jusqu'au milieu
        MOVEMENT::moveUntilLine();
    }
    
    void moveFromSquareToDropOff(int currentCol, int currentRow){
        if(currentCol < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveUntilLine();
        MOVEMENT::turnLeft(45);
        MOVEMENT::turnLeftUntilLine();

        // Ce rend SUR la première ligne
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(currentCol);
        MOVEMENT::turnLeft(45);
        MOVEMENT::turnLeftUntilLine();

        // Avance a l'origine
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(currentRow+1);

        //Tourne vers la zone
        MOVEMENT::turnRight(45);
        MOVEMENT::turnRightUntilLine();

        // Avance au dropoff
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByEndLine();

        // Tourne jusqu'a la ligne(180) pour regarder le NORD
        MOVEMENT::turnLeft(90);
        MOVEMENT::turnLeftUntilLine();
    }

    void moveFromDropOffToWhite(){
        // Ce rend devant le joueur blanc
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(5);

        // Tourne pour être à dos au joueur
        MOVEMENT::turnLeft(45);
        MOVEMENT::turnLeftUntilLine();
    }

    void moveFromDropOffToBlack(){
        // Ce rend à l'origine(0,0)
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(1);

        MOVEMENT::turnLeft(45);
        MOVEMENT::turnLeftUntilLine();

        // Avance au fond du tableau
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(8);

        // Avance devant le joueur
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(4);

        // Tourne pour être à dos au joueur
        MOVEMENT::turnRight(45);
        MOVEMENT::turnRightUntilLine();
    }

    void waitEndMoveByEndLine(){
        while(MOVEMENT::getCurrentMove() != MOVEMENT::MoveEnum::NONE){
            LINE::vCourseCorrection();
            MOVEMENT::runMovementController();

            if(LINE::ucReadLineSensors() == 0b000){
                MOVEMENT::stop();
            }
            
            delay(2);
        }
    }

    void waitEndMoveByLineNb(int nbLine){
        int detectedLines = 0;
        bool oldWasOnAllLine = false;

        while(MOVEMENT::getCurrentMove() != MOVEMENT::MoveEnum::NONE){
            LINE::vCourseCorrection();
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