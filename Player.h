#pragma once
#include "Maze.h"

class Player {
public:
    Player(int startX, int startY);
    void Move(int dx, int dy, const Maze& maze);
    int GetX() const;
    int GetY() const;

private:
    int x, y; 
}; 