#include "Arena.h"
#include <iostream>
#include <sstream>
#include <cctype>

Arena::Arena() : firstSnake(nullptr), lastSnake(nullptr), tickCount(0) {}

Arena::~Arena()
{
    // TODO: Free all remaining snakes.
    clearSnakes();
}

// ---------------------------------------------------------------- helpers (given)

// A valid snake id is a single uppercase letter A-Z.
static bool isSnakeId(const std::string &s)
{
    return s.size() == 1 && s[0] >= 'A' && s[0] <= 'Z';
}

// ---------------------------------------------------------------- player list

Snake *Arena::findSnake(char id) const
{
    // TODO: Return the ALIVE snake with this id, or nullptr.

    Snake *current = firstSnake;
    while(current != nullptr){
        if(current->getId() == id && current->isAlive() == true){
            return current;
        }
        current = current->getNext();
    }
    return nullptr;
}

void Arena::addSnake(Snake *s)
{
    // TODO: Append s to the end of the player list (turn order = spawn order).
    if (firstSnake == nullptr){
        firstSnake = s;
        lastSnake = s;
    }

    else{
        lastSnake->setNext(s);
        lastSnake = s;
    }
    
}

void Arena::clearSnakes()
{
    // TODO: Delete every snake in the player list, reset the list
    // and clear the leaderboard.

    Snake *current = firstSnake;
    while(current != nullptr){
        Snake *next = current->getNext();
        delete current;
        current = next;
    }
    firstSnake = nullptr;
    lastSnake = nullptr;
    leaderboard.clear();
}

void Arena::removeDeadSnakes()
{
    // TODO: Unlink and delete every snake with isAlive() == false.

    Snake *prev = nullptr;
    Snake *current = firstSnake;

    while(current != nullptr){
        if(current->isAlive() == false && current == firstSnake){
            if(firstSnake == lastSnake){
                delete current;
                firstSnake = nullptr;
                lastSnake = nullptr;
                return;
            }
            Snake *next = current->getNext();
            delete current;
            current = next;
            firstSnake = current;
        }

        else if(current->isAlive() == false && current != firstSnake && current != lastSnake){
            Snake *next = current->getNext();
            prev->setNext(current->getNext());
            delete current;
            current = next;
        }

        else if(current->isAlive() == false && current == lastSnake){
            prev->setNext(nullptr);
            delete current;
            current = nullptr;
            lastSnake = prev;
            
        }

        else{
            Snake *next = current->getNext();
            prev = current;
            current = next;
            
        }
    }
}

void Arena::killSnake(Snake *s)
{
    // TODO:
    //  - Print: std::cout << "Snake " << id << " died with length " << length
    //           << " and score " << score << "." << std::endl;
    //  - Every cell of its body becomes FOOD_CELL on the board.
    //  - Mark it dead and remove it from the leaderboard.
    //  Note: tick() may still be iterating over the player list.
    std::cout << "Snake " << s->getId() << " died with length " << s->getLength() << " and score " << s->getScore() << "." << std::endl;
    Segment *ptr = s->getBody().getHead();
    while(ptr != nullptr){
        Segment *nextPtr = ptr->getNext();
        board.set(ptr->getRow(), ptr->getCol(), FOOD_CELL);
        ptr = nextPtr;
    }
    s->setAlive(false);
    leaderboard.remove(s);

}

// ---------------------------------------------------------------- movement

void Arena::moveSnake(Snake *s)
{
    // TODO: Move s one cell forward. The rules and messages are in Section 4.6 of the assignment.
    // Messages:
    //   "Snake A hit a wall at (nr,nc)."
    //   "Snake A and Snake B collided head-on at (nr,nc)."
    //   "Snake A bit itself at (nr,nc)."
    //   "Snake A bit Snake B at (nr,nc). Snake B lost K segment(s)."
    //   "Snake A ate food at (nr,nc)."
    // The board must always show the head as the uppercase id, body as lowercase.
    int row, col;
    s->getNextHeadPosition(row,col);
    int tr, tc;
    Segment *oldHead = s->getBody().getHead();
    Segment *oldTail = s->getBody().getTail();
    

    if(!board.inBounds(row, col)){
        std::cout << "Snake " << s->getId() << " hit a wall at (" << row << "," << col << ")." << std::endl;
        killSnake(s);
        return;
    }

    else{
        char cellType = board.get(row,col);
        if(cellType == EMPTY_CELL || (row == oldTail->getRow() && col == oldTail->getCol())){
            board.set(oldHead->getRow(),oldHead->getCol(), tolower(s->getId()));
            s->getBody().popBack(tr,tc);
            board.set(tr, tc, EMPTY_CELL);
            s->getBody().pushFront(row,col);
            board.set(row,col, s->getId());
            return;
        }

        else if(cellType == FOOD_CELL){
            board.set(oldHead->getRow(),oldHead->getCol(), tolower(s->getId()));
            s->getBody().pushFront(row,col);
            board.set(row,col, s->getId());
            s->addScore(1);
            leaderboard.reposition(s);
            std::cout << "Snake " << s->getId() << " ate food at (" << row << "," << col << ")." << std::endl;
            return;
        }

        else if(cellType == WALL_CELL){
            std::cout << "Snake " << s->getId() << " hit a wall at (" << row << "," << col << ")." << std::endl;
            killSnake(s);
            return;
        }

        else if(cellType == tolower(s->getId()) && !(row == oldTail->getRow() && col == oldTail->getCol())){
            std::cout << "Snake " << s->getId() << " bit itself at (" << row << "," << col << ")." << std::endl;
            killSnake(s);
            return;
        }

        else if(islower(cellType)){
            Snake *hitSnake = findSnake(toupper(cellType));
            SnakeBody limb = hitSnake->getBody().splitAt(row,col);
            int limbLength = limb.getLength();

            Segment *ptr = limb.getHead();
            while(ptr != nullptr){
                Segment *nextPtr = ptr->getNext();
                board.set(ptr->getRow(), ptr->getCol(), FOOD_CELL);
                ptr = nextPtr;
            }

            std::cout << "Snake " << s->getId() << " bit Snake " << hitSnake->getId() << " at ("<< row << "," << col <<  "). Snake " << hitSnake->getId() << " lost " << limbLength << " segment(s)." << std::endl;

            board.set(oldHead->getRow(),oldHead->getCol(), tolower(s->getId()));
            s->getBody().pushFront(row,col);
            board.set(row,col, s->getId());
            s->addScore(1);
            leaderboard.reposition(s);
            std::cout << "Snake " << s->getId() << " ate food at (" << row << "," << col << ")." << std::endl;
            return;
        }

        else if(isupper(cellType)){
            Snake *hitSnake = findSnake(toupper(cellType));
            Segment *oldHitHead = hitSnake->getBody().getHead();
            std::cout << "Snake " << s->getId() << " and Snake " << hitSnake->getId() << " collided head-on at ("<< row << "," << col <<  ")." << std::endl;
            if(s->getLength() > hitSnake->getLength()){
                killSnake(hitSnake);
                board.set(oldHead->getRow(),oldHead->getCol(), tolower(s->getId()));
                s->getBody().pushFront(row,col);
                board.set(row,col, s->getId());
                s->addScore(1);
                leaderboard.reposition(s);
                std::cout << "Snake " << s->getId() << " ate food at (" << row << "," << col << ")." << std::endl;
            }

            else if(s->getLength() < hitSnake->getLength()){
                killSnake(s);
            }

            else{
                killSnake(s);
                killSnake(hitSnake);
            }
            return;
        }
    }
}

void Arena::tick()
{
    // TODO:
    //  - increase tickCount and print "--- Tick <tickCount> ---"
    //  - every alive snake moves once, in player list order
    //  - snakes that died during this tick must not move
    //  - dead snakes must be removed from the player list before the tick ends
    tickCount +=1;
    std::cout << "--- Tick " << tickCount << " ---" << std::endl;

    Snake *current = firstSnake;
    while(current != nullptr){
        if(current->isAlive()){
            moveSnake(current);
        }
        current = current->getNext();
    }
    removeDeadSnakes();
}

// ---------------------------------------------------------------- commands

void Arena::cmdInitBoard(const std::string &args)
{
    // TODO: INIT_BOARD <rows> <cols>   (the input is always well-formed)
    //  Resets everything: snakes, leaderboard, tick counter.
    //  Create the new board with move assignment:  board = Board(r, c);
    //  "Board initialized with <r> rows and <c> columns."
    std::istringstream ss(args);
    int r, c;
    ss >> r >> c;

    clearSnakes();
    tickCount = 0;
    board = Board(r, c);
    std::cout << "Board initialized with " << r << " rows and " << c << " columns." << std::endl;
}

// Given ready, as an example for parsing.
// (Checking the format is not required: the input is always well-formed.)
void Arena::cmdWall(const std::string &args)
{
    std::istringstream ss(args);
    int r, c;
    ss >> r >> c;
    if (!board.inBounds(r, c) || board.get(r, c) != EMPTY_CELL)
    {
        std::cout << "Error: Cell (" << r << "," << c << ") is not available." << std::endl;
        return;
    }
    board.set(r, c, WALL_CELL);
    std::cout << "Wall placed at (" << r << "," << c << ")." << std::endl;
}

void Arena::cmdFood(const std::string &args)
{
    // TODO: FOOD <r> <c>   (same rules as WALL)
    //  "Error: Cell (r,c) is not available."
    //  "Food placed at (r,c)."
    std::istringstream ss(args);
    int r, c;
    ss >> r >> c;

    if (!board.inBounds(r, c) || board.get(r, c) != EMPTY_CELL)
    {
        std::cout << "Error: Cell (" << r << "," << c << ") is not available." << std::endl;
        return;
    }
    board.set(r, c, FOOD_CELL);
    std::cout << "Food placed at (" << r << "," << c << ")." << std::endl;
}

void Arena::cmdSpawn(const std::string &args)
{
    // TODO: SPAWN <id> <r> <c> <dir> <len>
    //  The head is at (r,c), the body extends in the OPPOSITE direction of <dir>.
    //  alive snake with same id -> "Error: Snake <id> already exists."
    //  any cell out of bounds or not EMPTY_CELL -> "Error: Cannot spawn Snake <id> at (r,c)."
    //  Add to the player list AND to the leaderboard.
    //  "Snake <id> spawned at (r,c) facing <DIR> with length <len>."

    std::istringstream ss(args);
    std::string dirStr;
    char id;
    int r, c, len;

    ss >> id >> r >> c >> dirStr >> len;

    Direction dir;
    parseDirection(dirStr, dir);

    if(findSnake(id)!= nullptr){
        std::cout << "Error: Snake " << id << " already exists." << std::endl;
        return;
    }

    Direction oppDir = oppositeDirection(dir);
    int currentR = r;
    int currentC = c;

    for(int i = 0; i < len; i++){
        if (!board.inBounds(currentR, currentC) || board.get(currentR, currentC) != EMPTY_CELL){
        std::cout << "Error: Cannot spawn Snake " << id << " at (" << r << "," << c << ")." << std::endl;
        return;
        }
        currentR += rowDelta(oppDir);
        currentC += colDelta(oppDir);
    }

    Snake *newSnake = new Snake(id, dir);
    addSnake(newSnake);
    leaderboard.insertSorted(newSnake);

    currentR = r;
    currentC = c;

    for(int i = 0; i < len; i++){
        newSnake->getBody().pushBack(currentR, currentC);
        
        if(i==0){
            board.set(currentR, currentC, id);
        }
        else{
        board.set(currentR, currentC, tolower(id));
        }
        currentR += rowDelta(oppDir);
        currentC += colDelta(oppDir);
    }

    std::cout << "Snake " << id << " spawned at (" << r << "," << c << ") facing " << directionToString(dir) << " with length " << len << "." << std::endl;
}

void Arena::cmdTurn(const std::string &args)
{
    // TODO: TURN <id> <dir>
    //  "Error: Snake <id> not found."
    //  "Error: Snake <id> cannot turn back."
    //  "Snake <id> is now facing <DIR>."
    std::istringstream ss(args);
    std::string dirStr;
    char id;
    ss >> id >> dirStr;

    Direction dir;
    parseDirection(dirStr, dir);
    Snake *foundSnake = findSnake(id);

    if(foundSnake == nullptr){
        std::cout << "Error: Snake " << id << " not found." << std::endl;
        return;
    }
    
    else{
        
        if(!foundSnake->turn(dir)){
            std::cout << "Error: Snake " << id << " cannot turn back." << std::endl;
        }
        else{
            std::cout << "Snake " << id << " is now facing " << directionToString(dir) << "." << std::endl;
        }
    }
}

void Arena::cmdReverse(const std::string &args)
{
    // TODO: REVERSE <id>
    //  "Error: Snake <id> not found."
    //  Update the board too (old head becomes lowercase, new head uppercase).
    //  "Snake <id> reversed and is now facing <DIR>."

    std::istringstream ss(args);
    char id;
    ss >> id;

    Snake *foundSnake = findSnake(id);

    if(foundSnake == nullptr){
        std::cout << "Error: Snake " << id << " not found." << std::endl;
        return;
    }

    else{
        int oldHeadRow = foundSnake->getBody().getHead()->getRow();
        int oldHeadCol = foundSnake->getBody().getHead()->getCol();

        foundSnake->reverse();

        board.set(oldHeadRow, oldHeadCol, tolower(id));

        int newHeadRow = foundSnake->getBody().getHead()->getRow();
        int newHeadCol = foundSnake->getBody().getHead()->getCol();

        board.set(newHeadRow, newHeadCol, toupper(id));

        std::cout << "Snake " << id << " reversed and is now facing " << directionToString(foundSnake->getDirection()) <<"." << std::endl;
    }
}

void Arena::cmdTick(const std::string &args)
{
    // TODO: TICK [n]   (n is optional, default 1)
    std::istringstream ss(args);
    int n;
    
    if(!(ss >> n)){
        n = 1;
        tick();
    }
    else{
        for(int i = 0; i < n; i++){
            tick();
        }
    }
}

// Given ready
void Arena::printSnakes() const
{
    std::cout << "--- Snakes ---" << std::endl;
    if (!firstSnake)
    {
        std::cout << "No snakes in the arena." << std::endl;
        return;
    }
    for (Snake *cur = firstSnake; cur; cur = cur->getNext())
    {
        std::cout << cur->getId() << " " << directionToString(cur->getDirection())
                  << " length=" << cur->getLength() << " score=" << cur->getScore()
                  << ": " << cur->getBody() << std::endl;
    }
}

// Given ready
void Arena::processCommand(const std::string &line)
{
    std::istringstream ss(line);
    std::string cmd;
    if (!(ss >> cmd))
        return;
    std::string args;
    std::getline(ss, args);

    if (cmd == "INIT_BOARD")
    {
        cmdInitBoard(args);
        return;
    }

    bool knownBoardCmd = cmd == "WALL" || cmd == "FOOD" || cmd == "SPAWN" || cmd == "TURN" ||
                         cmd == "REVERSE" || cmd == "TICK" || cmd == "PRINT_BOARD" ||
                         cmd == "PRINT_SNAKES" || cmd == "PRINT_LEADERBOARD";
    if (!knownBoardCmd)
    {
        std::cout << "Error: Unknown command '" << cmd << "'." << std::endl;
        return;
    }
    if (!board.isInitialized())
    {
        std::cout << "Error: Board not initialized." << std::endl;
        return;
    }

    if (cmd == "WALL")
        cmdWall(args);
    else if (cmd == "FOOD")
        cmdFood(args);
    else if (cmd == "SPAWN")
        cmdSpawn(args);
    else if (cmd == "TURN")
        cmdTurn(args);
    else if (cmd == "REVERSE")
        cmdReverse(args);
    else if (cmd == "TICK")
        cmdTick(args);
    else if (cmd == "PRINT_BOARD")
        board.print();
    else if (cmd == "PRINT_SNAKES")
        printSnakes();
    else if (cmd == "PRINT_LEADERBOARD")
        leaderboard.print();
}
