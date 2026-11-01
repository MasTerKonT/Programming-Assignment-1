#include "Leaderboard.h"
#include <iostream>

Leaderboard::~Leaderboard() { clear(); }

void Leaderboard::insertSorted(Snake *s)
{
    // TODO: Insert s so the list stays sorted:
    //   1) higher score first
    //   2) equal scores -> smaller id first ('A' before 'B')

    RankNode *node = new RankNode(s);

    if(head == nullptr || s->getScore() > head->snake->getScore() || (s->getScore() == head->snake->getScore() && s->getId() < head->snake->getId())){
        node->next = head;
        head = node;
    }

    else{
        RankNode *ptr = head;
        while(ptr->next != nullptr && (s->getScore() < ptr->next->snake->getScore() || (s->getScore() == ptr->next->snake->getScore() && s->getId() > ptr->next->snake->getId()))){
            ptr = ptr->next;
        }
        node->next = ptr->next;
        ptr->next = node;
        
    }
    size +=1;
}

bool Leaderboard::remove(Snake *s)
{
    // TODO: Find the node whose snake == s, unlink and delete the NODE.
    // Do NOT delete the snake itself; the Arena owns the snakes.
    // Return false if not found.

    if(head == nullptr){
        return false;
    }

    if(head->snake == s){
        RankNode *removal = head;
        head = head->next;
        delete removal;
        size -=1;
        return true;
    }

    RankNode *ptr = head;

    while(ptr->next != nullptr){
        if(ptr->next->snake == s){
            RankNode *removal = ptr->next;
            ptr->next = removal->next;
            removal->next = nullptr;
            delete removal;
            size -=1;
            return true;
            
        }
        ptr = ptr->next;
    }
    return false;
}

void Leaderboard::reposition(Snake *s)
{
    // TODO: Called after s's score changed. Put s back at its correct place.
    remove(s);
    insertSorted(s);
}

void Leaderboard::clear()
{
    // TODO: Delete all RankNodes (not the snakes).
    RankNode *ptr = head;

    while (ptr != nullptr){
        RankNode *nextPtr = ptr->next;
        delete ptr;
        ptr = nextPtr;
    }

    head = nullptr;
    size = 0;
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
