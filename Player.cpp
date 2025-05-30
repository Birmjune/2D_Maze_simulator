#include "Player.h"

Player::Player(int startX, int startY) : x(startX), y(startY) {}

void Player::Move(int dx, int dy, const Maze& maze) { // 벽이 아닌 곳으로 이동하고자 하는 경우에만 이동 + Maze 값은 안 바꿀 거니 상수 참조
    int newX = x + dx;
    int newY = y + dy;
    if (newX >= 0 && newX < maze.GetSize() && newY >= 0 && newY < maze.GetSize() && maze.GetPlaceType(newX, newY) != WALL) {
        x = newX;
        y = newY;
    } // 미로 내부에서 벽이 아닌 곳으로 이동하려 하는 경우, 위치를 변경
}

int Player::GetX() const { return x; }
int Player::GetY() const { return y; }