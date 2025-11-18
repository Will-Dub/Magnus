#include "ChessMovement.h"

namespace CHESS_MOVEMENT{
    void moveFromWhiteToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        if(destRow != 0){
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destRow);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
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
        MOVEMENT::moveForward(ROBUS_RADIUS_CM);

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

        MOVEMENT::moveForward(ROBUS_RADIUS_CM);
        MOVEMENT::turnLeft(45);
        MOVEMENT::turnLeftUntilLine();

        // Ce rend SUR la première ligne
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(currentCol);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM);
        MOVEMENT::turnLeft(45);
        MOVEMENT::turnLeftUntilLine();

        // Avance a l'origine
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(currentRow+1);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM);

        //Tourne vers la zone
        MOVEMENT::turnRight(45);
        MOVEMENT::turnRightUntilLine();

        // Avance au dropoff
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByEndLine();

        // Tourne jusqu'a la ligne(180) pour regarder l'EST
        MOVEMENT::turnLeft(90);
        MOVEMENT::turnLeftUntilLine();
    }

    void moveFromDropOffToWhite(){
        // Ce rend devant le joueur blanc
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(5);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM);

        // Tourne pour être à dos au joueur
        MOVEMENT::turnLeft(45);
        MOVEMENT::turnLeftUntilLine();
    }

    void moveFromDropOffToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        // Assume que le robot regarde l'est
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(destRow);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM);
        MOVEMENT::turnLeft(45);
        MOVEMENT::turnLeftUntilLine();

        // Ce rend SUR la ligne
        if(destRow != 0){
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destRow);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM);
        }

        // Tourne pour regarder l'EST
        MOVEMENT::turnRight(45);
        MOVEMENT::turnRightUntilLine();

        // Avance jusqu'au milieu de la case
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(destCol+1);

        // Tourne pour regarder le NORD
        MOVEMENT::turnLeft(85);

        // Avance jusqu'au milieu
        MOVEMENT::moveUntilLine();
    }

    void moveFromSquareToBlack(int currentCol, int currentRow){
        if(currentCol < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForward(ROBUS_RADIUS_CM);
        
        // Va à la ligne du milieu
        if(currentCol <= 3){
            MOVEMENT::turnRight(45);
            MOVEMENT::turnRightUntilLine();

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(4 - currentCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM);

            MOVEMENT::turnLeft(45);
            MOVEMENT::turnLeftUntilLine();
        }else{
            MOVEMENT::turnLeft(45);
            MOVEMENT::turnLeftUntilLine();

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(currentCol - 3);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM);

            MOVEMENT::turnRight(45);
            MOVEMENT::turnRightUntilLine();
        }

        // Va devant le joueur
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(7 - currentRow);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM);

        // Tourne à dos(180 degree)
        MOVEMENT::turnLeft(125);
        MOVEMENT::turnLeftUntilLine();
    }

    void moveFromSquareToWhite(int currentCol, int currentRow){
        if(currentCol < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForward(ROBUS_RADIUS_CM);
        
        // Va à la ligne du milieu
        if(currentCol <= 3){
            MOVEMENT::turnRight(45);
            MOVEMENT::turnRightUntilLine();

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(4 - currentCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM);

            MOVEMENT::turnRight(45);
            MOVEMENT::turnRightUntilLine();
        }else{
            MOVEMENT::turnLeft(45);
            MOVEMENT::turnLeftUntilLine();

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(currentCol - 3);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM);

            MOVEMENT::turnLeft(45);
            MOVEMENT::turnLeftUntilLine();
        }

        // Va devant le joueur
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(currentRow + 1);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM);

        // Tourne à dos(180 degree)
        MOVEMENT::turnLeft(125);
        MOVEMENT::turnLeftUntilLine();
    }

    void moveSquareToSquare(int currentCol, int currentRow, int destCol, int destRow){
        if(destCol < 0 || currentCol < 0 || destRow < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForward(ROBUS_RADIUS_CM);

        if(currentCol >= destCol){
            MOVEMENT::turnLeft(45);
            MOVEMENT::turnLeftUntilLine();

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(currentCol - destCol + 1);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM);

            if(destRow > currentRow+1){
                MOVEMENT::turnRight(45);
                MOVEMENT::turnRightUntilLine();

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(destRow - currentRow - 1);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM);

                MOVEMENT::turnRight(45);
                MOVEMENT::turnRightUntilLine();
            }else if(destRow <= currentRow){
                MOVEMENT::turnLeft(45);
                MOVEMENT::turnLeftUntilLine();

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(currentRow - destRow + 1);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM);

                MOVEMENT::turnLeft(45);
                MOVEMENT::turnLeftUntilLine();
            }else{
                MOVEMENT::turnRight(125);
                MOVEMENT::turnRightUntilLine();
            }
        }else{
            MOVEMENT::turnRight(45);
            MOVEMENT::turnRightUntilLine();

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destCol - currentCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM);

            if(destRow > currentRow+1){
                MOVEMENT::turnLeft(45);
                MOVEMENT::turnLeftUntilLine();

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(destRow - currentRow - 1);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM);

                MOVEMENT::turnRight(45);
                MOVEMENT::turnRightUntilLine();
            }else if(destRow <= currentRow){
                MOVEMENT::turnRight(45);
                MOVEMENT::turnRightUntilLine();

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(currentRow - destRow + 1);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM);

                MOVEMENT::turnLeft(45);
                MOVEMENT::turnLeftUntilLine();
            }else{
                // Face déjà l'EST
            }
        }

        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(1);

        MOVEMENT::turnLeft(85);
        MOVEMENT::moveUntilLine();
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