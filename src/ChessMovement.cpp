#include "ChessMovement.h"

namespace CHESS_MOVEMENT{
    void moveFromWhiteToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        if(destCol < 4){
            MOVEMENT::turnLeft(65);
            MOVEMENT::turnLeftUntilLine(true);
            
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(4 - destCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            MOVEMENT::turnRight(65);
            MOVEMENT::turnRightUntilLine(true);
        }else if(destCol > 4){
            MOVEMENT::turnRight(65);
            MOVEMENT::turnRightUntilLine(true);
            
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destCol - 4);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            MOVEMENT::turnLeft(65);
            MOVEMENT::turnLeftUntilLine(true);
        }

        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(destRow+1);
    }

    void moveFromBlackToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(8-destRow);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        if(destCol < 4){
            MOVEMENT::turnRight(65);
            MOVEMENT::turnRightUntilLine(true);
            
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(4 - destCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            MOVEMENT::turnRight(65);
            MOVEMENT::turnRightUntilLine(true);
        }else if(destCol > 4){
            MOVEMENT::turnLeft(65);
            MOVEMENT::turnLeftUntilLine(true);
            
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destCol - 4);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            MOVEMENT::turnLeft(65);
            MOVEMENT::turnLeftUntilLine(true);
        }

        // Avance jusqu'au milieu
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(1);
    }
    
    void moveFromSquareToDropOff(int currentCol, int currentRow){
        if(currentCol < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        MOVEMENT::turnLeft(65);
        MOVEMENT::turnLeftUntilLine(true);

        // Ce rend SUR la première ligne
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(currentCol);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        MOVEMENT::turnLeft(65);
        MOVEMENT::turnLeftUntilLine(true);

        // Avance au dropoff
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(currentRow+1);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        //Tourne pour être vers l'est
        MOVEMENT::turnLeft(65);
        MOVEMENT::turnLeftUntilLine(true);
    }

    void moveFromDropOffToWhite(){
        // Ce rend devant le joueur blanc
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(4);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        // Tourne pour être à dos au joueur
        MOVEMENT::turnLeft(65);
        MOVEMENT::turnLeftUntilLine(true);
    }

    void moveFromDropOffToSquare(int destCol, int destRow){
        if(destCol < 0 || destRow < 0){
            return;
        }

        // Assume que le robot regarde l'est

        if(destCol != 0){
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(destCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        }

        MOVEMENT::turnLeft(65);
        MOVEMENT::turnLeftUntilLine(true);

        // Ce rend SUR la ligne
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(destRow+1);
    }

    void moveFromSquareToBlack(int currentCol, int currentRow){
        if(currentCol < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(8 - currentRow);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        
        // Va à la ligne du milieu
        if(currentCol < 4){
            MOVEMENT::turnRight(65);
            MOVEMENT::turnRightUntilLine(true);

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(4 - currentCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            MOVEMENT::turnRight(65);
            MOVEMENT::turnRightUntilLine(true);
        }else if(currentCol > 4){
            MOVEMENT::turnLeft(65);
            MOVEMENT::turnLeftUntilLine(true);

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(currentCol - 4);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            MOVEMENT::turnLeft(65);
            MOVEMENT::turnLeftUntilLine(true);
        }else{
            // Tourne à dos(180 degree)
            MOVEMENT::turnLeft(135);
            MOVEMENT::turnLeftUntilLine(true);
        }
    }

    void moveFromSquareToWhite(int currentCol, int currentRow){
        if(currentCol < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        MOVEMENT::turnLeft(135);
        MOVEMENT::turnLeftUntilLine(true);

        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(currentRow + 1);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
        
        // Va à la ligne du milieu
        if(currentCol < 4){
            MOVEMENT::turnLeft(65);
            MOVEMENT::turnLeftUntilLine(true);

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(4 - currentCol);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            MOVEMENT::turnLeft(65);
            MOVEMENT::turnLeftUntilLine(true);
        }else if(currentCol > 4){
            MOVEMENT::turnRight(65);
            MOVEMENT::turnRightUntilLine(true);

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(currentCol - 4);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            MOVEMENT::turnRight(65);
            MOVEMENT::turnRightUntilLine(true);
        }else{
            // Tourne à dos(180 degree)
            MOVEMENT::turnLeft(135);
            MOVEMENT::turnLeftUntilLine(true);
        }
    }

    void moveSquareToSquare(int currentCol, int currentRow, int destCol, int destRow){
        if(destCol < 0 || currentCol < 0 || destRow < 0 || currentRow < 0){
            return;
        }

        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        if(destRow < currentRow){
            MOVEMENT::turnRight(135);
            MOVEMENT::turnRightUntilLine(true);

            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(1+currentRow-destRow);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            if(destCol < currentCol){
                MOVEMENT::turnRight(65);
                MOVEMENT::turnRightUntilLine(true);

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(currentCol-destCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                MOVEMENT::turnRight(65);
                MOVEMENT::turnRightUntilLine(true);
            }else if(destCol > currentCol){
                MOVEMENT::turnLeft(65);
                MOVEMENT::turnLeftUntilLine(true);

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(destCol-currentCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                MOVEMENT::turnLeft(65);
                MOVEMENT::turnLeftUntilLine(true);
            }else{
                MOVEMENT::turnRight(135);
                MOVEMENT::turnRightUntilLine(true);
            }
        }else if(destRow > currentRow){
            if(destRow != currentRow + 1){
                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(destRow-currentRow-1);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);
            }

            if(destCol < currentCol){
                MOVEMENT::turnLeft(65);
                MOVEMENT::turnLeftUntilLine(true);

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(currentCol-destCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                MOVEMENT::turnRight(65);
                MOVEMENT::turnRightUntilLine(true);
            }else if(destCol > currentCol){
                MOVEMENT::turnRight(65);
                MOVEMENT::turnRightUntilLine(true);

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(destCol-currentCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                MOVEMENT::turnLeft(65);
                MOVEMENT::turnLeftUntilLine(true);
            }else{
                // NOTHING its in front
            }
        }else{
            MOVEMENT::turnLeft(135);
            MOVEMENT::turnLeftUntilLine(true);
            MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
            waitEndMoveByLineNb(1);
            MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

            if(destCol < currentCol){
                MOVEMENT::turnRight(65);
                MOVEMENT::turnRightUntilLine(true);

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(currentCol-destCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                MOVEMENT::turnRight(65);
                MOVEMENT::turnRightUntilLine(true);
            }else if(destCol > currentCol){
                MOVEMENT::turnLeft(65);
                MOVEMENT::turnLeftUntilLine(true);

                MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
                waitEndMoveByLineNb(destCol-currentCol);
                MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

                MOVEMENT::turnLeft(65);
                MOVEMENT::turnLeftUntilLine(true);
            }else{
                // NOTHING its in front
            }
        }

        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(1);
    }

    void moveFromBlackToWhite(){
        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(8);
        MOVEMENT::moveForward(ROBUS_RADIUS_CM/2);

        MOVEMENT::turnLeft(155);
        MOVEMENT::turnLeftUntilLine(true);
    }

    void waitEndMoveByLineNb(int nbLine){
        int detectedLines = 0;
        int detectedLineReadCount = 0;
        bool oldWasOnAllLine = false;

        while(MOVEMENT::getCurrentMove() != MOVEMENT::MoveEnum::NONE){
            LINE::vCourseCorrection();
            MOVEMENT::runMovementController();

            if(LINE::ucReadLineSensors() == 0b111){
                detectedLineReadCount++;
            }else{
                detectedLineReadCount = 0;
                oldWasOnAllLine = false;
            }

            if(!oldWasOnAllLine && detectedLineReadCount >= MIN_DETECTED_LINE_READ_COUNT){
                detectedLines++;
                oldWasOnAllLine = true;
            }

            if(detectedLines >= nbLine){
                MOVEMENT::stop();
            }
            
            delay(2);
        }
    }
}