#include "ChessMovement.h"

namespace CHESS_MOVEMENT{
    void moveFromWhiteToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        if(destCol < 4){
            turnLeft();
            
            moveByLineNb(4 - destCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            turnRight();
        }else if(destCol > 4){
            turnRight();
            
            moveByLineNb(destCol - 4);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            turnLeft();
        }

        moveByLineNb(destRow + 1);
    }

    void moveFromBlackToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        moveByLineNb(8 - destRow);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        if(destCol < 4){
            turnRight();
            
            moveByLineNb(4 - destCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            turnRight();
        }else if(destCol > 4){
            turnLeft();
            
            moveByLineNb(destCol - 4);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            turnLeft();
        }

        // Avance jusqu'au milieu
        moveByLineNb(1);
    }
    
    void moveFromSquareToDropOff(int currentCol, int currentRow){
        if(currentCol < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        if(currentCol != 0){
            turnLeft();

            // Ce rend SUR la première ligne
            moveByLineNb(currentCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            turnLeft();
        }else{
            fullTurn();
        }

        // Avance au dropoff
        moveByLineNb(currentRow + 1);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        //Tourne pour être vers l'est
        turnLeft();
    }

    void moveFromDropOffToWhite(){
        // Ce rend devant le joueur blanc
        moveByLineNb(4);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        // Tourne pour être à dos au joueur
        turnLeft();
    }

    void moveFromDropOffToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        // Assume que le robot regarde l'est

        if(destCol != 0){
            moveByLineNb(destCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        }

        turnLeft();

        // Ce rend SUR la ligne
        moveByLineNb(destRow + 1);
    }

    void moveFromSquareToBlack(int currentCol, int currentRow){
        if(currentCol < 0 || currentRow < 0){
            return;
        }

        moveByLineNb(7 - currentRow);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        
        // Va à la ligne du milieu
        if(currentCol < 4){
            turnRight();

            moveByLineNb(4 - currentCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            turnRight();
        }else if(currentCol > 4){
            turnLeft();

            moveByLineNb(currentCol - 4);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            turnLeft();
        }else{
            fullTurn();
        }
    }

    void moveFromSquareToWhite(int currentCol, int currentRow){
        if(currentCol < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        fullTurn();

        moveByLineNb(currentRow + 1);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        
        // Va à la ligne du milieu
        if(currentCol < 4){
            turnLeft();

            moveByLineNb(4 - currentCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            turnLeft();
        }else if(currentCol > 4){
            turnRight();

            moveByLineNb(currentCol - 4);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            turnRight();
        }else{
            fullTurn();
        }
    }

    void moveSquareToSquare(int currentCol, int currentRow, int destCol, int destRow){
        if(destCol < 0 || currentCol < 0 || destRow < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        if(destRow < currentRow){
            fullTurn();

            moveByLineNb(1+currentRow-destRow);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            if(destCol < currentCol){
                turnRight();

                moveByLineNb(currentCol-destCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                turnRight();
            }else if(destCol > currentCol){
                turnLeft();

                moveByLineNb(destCol - currentCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                turnLeft();
            }else{
                fullTurn();
            }
        }else if(destRow > currentRow){
            if(destRow != currentRow + 1){
                moveByLineNb(destRow - currentRow - 1);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
            }

            if(destCol < currentCol){
                turnLeft();

                moveByLineNb(currentCol - destCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                turnRight();
            }else if(destCol > currentCol){
                turnRight();

                moveByLineNb(destCol - currentCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                turnLeft();
            }else{
                // NOTHING its in front
            }
        }else{
            fullTurn();
            moveByLineNb(1);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            if(destCol < currentCol){
                turnRight();

                moveByLineNb(currentCol - destCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                turnRight();
            }else if(destCol > currentCol){
                turnLeft();

                moveByLineNb(destCol - currentCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                turnLeft();
            }else{
                // NOTHING its in front
            }
        }

        moveByLineNb(1);
    }

    void moveFromBlackToWhite(){
        moveByLineNb(8);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        fullTurn();
    }

    void waitEndMoveByLineNb(int nbLine){
        int detectedLines = 0;
        int detectedLineReadCount = 0;
        int offLineCounter = 0;
        bool oldWasOnAllLine = false;

        while(MOVEMENT::getCurrentMove() != MOVEMENT::MoveEnum::NONE){
            LINE::vCourseCorrection();
            MOVEMENT::runMovementController();

            if(LINE::ucReadLineSensors() == 0b111){
                detectedLineReadCount++;
            }else{
                offLineCounter++;
                detectedLineReadCount = 0;
                oldWasOnAllLine = false;
            }

            if (!oldWasOnAllLine
                && detectedLineReadCount >= MIN_DETECTED_LINE_READ_COUNT
                && offLineCounter >= MIN_OFF_LINE_BEFORE_NEXT)
            {
                offLineCounter = 0;
                detectedLines++;
                oldWasOnAllLine = true;
                MOVEMENT::stop();
                delay(250);
                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            }

            if(detectedLines >= nbLine){
                MOVEMENT::stop();
            }
            
            delay(2);
        }
    }

    inline void turnLeft(){
        MOVEMENT::turnLeft(TURN_ANGLE);
        MOVEMENT::turnLeftUntilLine(true);
    }

    inline void turnRight(){
        MOVEMENT::turnRight(TURN_ANGLE);
        MOVEMENT::turnRightUntilLine(true);
    }

    inline void fullTurn(){
        MOVEMENT::turnLeft(FULL_TURN_ANGLE);
        MOVEMENT::turnLeftUntilLine(true);
    }

    inline void moveByLineNb(int nbLine){
        if(nbLine == 0){
            return;
        }

        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(nbLine);
    }
}