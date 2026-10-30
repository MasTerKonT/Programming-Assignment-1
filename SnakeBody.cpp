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
    return true;
}

void SnakeBody::pushFront(int row, int col)
{
    // TODO: Add a new segment at (row,col) as the new head.
}

void SnakeBody::pushBack(int row, int col)
{
    // TODO: Add a new segment at (row,col) as the new tail.
}

bool SnakeBody::popBack(int &row, int &col)
{
    // TODO: Remove the tail segment, write its coordinates into row/col and delete it.
    // Return false if the body is empty.
    return false;
}

void SnakeBody::reverse()
{
    // TODO: Reverse the list in place (do not create or delete segments).
}

Segment *SnakeBody::findAt(int row, int col) const
{
    // TODO: Return the segment at (row,col), or nullptr if there is none.
    return nullptr;
}

SnakeBody SnakeBody::splitAt(int row, int col)
{
    SnakeBody cut; // returned by value -> moved, not copied

    // TODO: The segment at (row,col) and every segment after it (towards the tail)
    // are moved into 'cut'; this body keeps the segments before it.
    // If there is no segment at (row,col), return an empty body.

    return cut;
}

void SnakeBody::clear()
{
    // TODO: Delete every segment. The body must be empty and usable afterwards.
    Segment *ptr = head;

    while (ptr != nullptr){
        Segment *nextptr = ptr->getNext();
        delete ptr;
        ptr = nextptr;
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
