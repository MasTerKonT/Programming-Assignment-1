#include "SnakeBody.h"

SnakeBody::~SnakeBody() { clear(); }

SnakeBody::SnakeBody(SnakeBody &&other) noexcept
    : head(nullptr), tail(nullptr), length(0)
{
    // TODO: Take over other's segments. 'other' must be left empty but valid.
    head = other.head;
    tail = other.tail;
    length = other.length;

    other.head = nullptr;
    other.tail = nullptr;
    other.length = 0;
}

SnakeBody &SnakeBody::operator=(SnakeBody &&other) noexcept
{
    // TODO: Same as the move constructor, but this body may already own segments
    // (they must not leak).
    if (this == &other){
        return *this;
    }

    clear();
    
    head = other.head;
    tail = other.tail;
    length = other.length;

    other.head = nullptr;
    other.tail = nullptr;
    other.length = 0;

    return *this;
}

bool SnakeBody::isEmpty() const
{
    // TODO: true if the body has no segments.
    return (head == nullptr);
    
}

void SnakeBody::pushFront(int row, int col)
{
    // TODO: Add a new segment at (row,col) as the new head.
    Segment *newHead = new Segment(row, col);
    
    if (length == 0){
        head = newHead;
        tail = newHead;
    }

    else {
        newHead->setNext(head);
        head->setPrev(newHead);
        head = newHead;
    }
    length +=1;
    return;

}

void SnakeBody::pushBack(int row, int col)
{
    // TODO: Add a new segment at (row,col) as the new tail.
    Segment *newTail = new Segment(row, col);
    
    if (length == 0){
        head = newTail;
        tail = newTail;
    }

    else {
        tail->setNext(newTail);
        newTail->setPrev(tail);
        tail = newTail;
    }
    length +=1;
    return;
}

bool SnakeBody::popBack(int &row, int &col)
{
    // TODO: Remove the tail segment, write its coordinates into row/col and delete it.
    // Return false if the body is empty.
    if (length == 0){
        return false;
    }
    else {
        Segment *oldTail = tail;
        row = tail->getRow();
        col = tail->getCol();
        
        if (length ==1){
            head = nullptr;
            tail = nullptr;
            length = 0;
            delete oldTail;
        }

        else {
            Segment *preTail = tail->getPrev();
            tail = preTail;
            tail->setNext(nullptr);
            delete oldTail;
            length -=1; 
        }
        return true;
    }


    return false;
}

void SnakeBody::reverse()
{
    // TODO: Reverse the list in place (do not create or delete segments).
    Segment *ptr = head;

    while(ptr != nullptr){
        Segment *saved = ptr->getNext();
        Segment *temp1 = ptr->getPrev();

        ptr->setPrev(saved);
        ptr->setNext(temp1);

        ptr = saved;
    }

    Segment *temp2 = head;
    head = tail;
    tail = temp2;
}

Segment *SnakeBody::findAt(int row, int col) const
{
    // TODO: Return the segment at (row,col), or nullptr if there is none.
    Segment *ptr = head;

    while (ptr != nullptr){
        if (ptr->getRow() == row && ptr->getCol() == col){
            return ptr;
        }
        ptr = ptr->getNext();
    }

    return nullptr;
}

SnakeBody SnakeBody::splitAt(int row, int col)
{
    SnakeBody cut; // returned by value -> moved, not copied

    // TODO: The segment at (row,col) and every segment after it (towards the tail)
    // are moved into 'cut'; this body keeps the segments before it.
    // If there is no segment at (row,col), return an empty body.

    Segment *found = findAt(row,col);
    Segment *ptr = head;
    int count = 0;

    if (found == nullptr){
        return cut;
    }

    if (found == head){
        cut.head = head;
        cut.tail = tail;
        cut.length = length - count;

        head = nullptr;
        tail = nullptr;
        length = 0;
        return cut;
    }

    while (ptr != nullptr){
        if (ptr == found){
            cut.head = found;
            cut.tail = tail;
            cut.length = length - count;

            tail = found->getPrev();
            tail->setNext(nullptr);
            found->setPrev(nullptr);
            length = count;
            return cut;
        }

        ptr = ptr->getNext();
        count +=1;
    }

    return cut;
}

void SnakeBody::clear()
{
    // TODO: Delete every segment. The body must be empty and usable afterwards.
    Segment *ptr = head;

    while (ptr != nullptr){
        Segment *nextPtr = ptr->getNext();
        delete ptr;
        ptr = nextPtr;
    }

    head = nullptr;
    tail = nullptr;
    length = 0;
}

// Given ready
std::ostream &operator<<(std::ostream &os, const SnakeBody &b)
{
    for (Segment *cur = b.head; cur; cur = cur->getNext())
    {
        os << *cur;
        if (cur->getNext())
            os << " ";
    }
    return os;
}
