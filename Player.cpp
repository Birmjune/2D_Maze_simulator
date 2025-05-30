#include "Player.h"

Player::Player(int startX, int startY) : x(startX), y(startY) {}

void Player::Move(int dx, int dy, const Maze& maze) { // ���� �ƴ� ������ �̵��ϰ��� �ϴ� ��쿡�� �̵� + Maze ���� �� �ٲ� �Ŵ� ��� ����
    int newX = x + dx;
    int newY = y + dy;
    if (newX >= 0 && newX < maze.GetSize() && newY >= 0 && newY < maze.GetSize() && maze.GetPlaceType(newX, newY) != WALL) {
        x = newX;
        y = newY;
    } // �̷� ���ο��� ���� �ƴ� ������ �̵��Ϸ� �ϴ� ���, ��ġ�� ����
}

int Player::GetX() const { return x; }
int Player::GetY() const { return y; }