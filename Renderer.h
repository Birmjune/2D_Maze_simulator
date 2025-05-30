#pragma once
#include "Maze.h"
#include "Player.h"
#using <System.Drawing.dll>

class Renderer {
public:
    Renderer(int cellSize);
    void Render2D(const Maze& maze, const Player* player, System::Drawing::Graphics^ g); // Maze�� ���� ����� form�� �׷��ִ� �Լ� 
    void ShowClearMessage(System::Drawing::Graphics^ g); // �̷θ� ���� clear �޼��� ����

private:
    int cellSize;  // �� ũ�� (�ϳ��� ĭ�� ũ��, �׸��׸��� �ȼ� ��)
};
