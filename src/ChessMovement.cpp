#include "ChessMovement.h"

namespace CHESS_MOVEMENT{
    void moveTo(int col, int row){
        // Assume its facing north
        int colPassed = 0;
        int rowPassed = 0;

        MOVEMENT::moveForwardNonBlocking(MAX_MOVE_DISTANCE);
        waitEndMoveByLineNb(row);

        MOVEMENT::turnRightNonBlocking(45);
        MOVEMENT::turnRightUntilLine();

        waitEndMoveByLineNb(col);
    }

    void waitEndMoveByLineNb(int nbLine){
        int detectedLines = 0;
        bool oldWasOnAllLine = false;

        while(MOVEMENT::getCurrentMove() != MOVEMENT::MoveEnum::NONE){
            MOVEMENT::runMovementController();

            if(!oldWasOnAllLine && LINE)
            
            delay(2);
        }
    }
}