#include "Leaderboard.h"
#include <iostream>

Leaderboard::~Leaderboard() { clear(); }

void Leaderboard::insertSorted(Snake *s)
{
    // TODO: Insert s so the list stays sorted:
    //   1) higher score first
    //   2) equal scores -> smaller id first ('A' before 'B')
}

bool Leaderboard::remove(Snake *s)
{
    // TODO: Find the node whose snake == s, unlink and delete the NODE.
    // Do NOT delete the snake itself; the Arena owns the snakes.
    // Return false if not found.
    return false;
}

void Leaderboard::reposition(Snake *s)
{
    // TODO: Called after s's score changed. Put s back at its correct place.
}

void Leaderboard::clear()
{
    // TODO: Delete all RankNodes (not the snakes).
}

// Given ready
void Leaderboard::print() const
{
    std::cout << "--- Leaderboard ---" << std::endl;
    if (!head)
    {
        std::cout << "No snakes in the arena." << std::endl;
        return;
    }
    int rank = 1;
    for (RankNode *cur = head; cur; cur = cur->next, ++rank)
    {
        std::cout << rank << ". " << cur->snake->getId()
                  << " score=" << cur->snake->getScore()
                  << " length=" << cur->snake->getLength() << std::endl;
    }
}
