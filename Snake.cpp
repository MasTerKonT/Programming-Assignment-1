#include "Snake.h"

Snake::Snake(char _id, Direction _direction)
    : id(_id), direction(_direction), score(0), alive(true), next(nullptr) {}

bool Snake::turn(Direction newDir)
{
    // TODO: The second segment is the "neck". If moving one cell in newDir would put
    // the head onto the neck, return false and change nothing. Otherwise set the
    // direction and return true. (Length 1: no neck, every turn is valid.)
    Segment *head = body.getHead();
    
    if (getLength() == 1){
        direction =  newDir;
        return true;
    }

    Segment *neck = head->getNext();
    int r = head->getRow();
    int c = head->getCol();

    switch(newDir)
    {
        case Direction::UP:
            r -= 1;
            break;
        case Direction::DOWN:
            r += 1;
            break;
        case Direction::LEFT:
            c -= 1;
            break;
        case Direction::RIGHT:
            c += 1;
            break;
    }
    if(neck->getRow() == r && neck->getCol() == c){
        return false;
    }

    direction = newDir;
    return true;
}

void Snake::reverse()
{
    // TODO: Reverse the body. New direction: from the new second segment towards the new head.
    //   e.g. head (2,1), second (2,2)  -> LEFT
    // If the snake has length 1, the direction just becomes the opposite.
    
    if (getLength() == 1){
        direction = oppositeDirection(direction);
        return;
    }

    body.reverse();

    Segment *head = body.getHead();
    Segment *neck = head->getNext();

    int dr = head->getRow() - neck->getRow();
    int dc = head->getCol() - neck->getCol();

    if (dr == 1){
        direction = Direction::DOWN;
    }
    else if (dr == -1){
        direction = Direction::UP;
    }
    else if (dc == 1){
        direction = Direction::RIGHT;
    }
    else if (dc == -1){
        direction = Direction::LEFT;
    }

}

void Snake::getNextHeadPosition(int &row, int &col) const
{
    // TODO: The cell the head would move into with the current direction.
    Segment *head = body.getHead();

    row = head->getRow();
    col = head->getCol();

    switch(direction)
    {
        case Direction::UP:
            row -= 1;
            break;
        case Direction::DOWN:
            row += 1;
            break;
        case Direction::LEFT:
            col -= 1;
            break;
        case Direction::RIGHT:
            col += 1;
            break;
    }
}
