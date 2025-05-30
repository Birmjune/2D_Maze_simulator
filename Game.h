#pragma once
#include "Maze.h"
#include "Player.h"
#include "Renderer.h"
#using <System.Drawing.dll>

class Game {
public:
    Game(int cellSize, int difficulty); 
    Game(Maze* editedMaze, int cellSize, System::Drawing::Point startPosition); // ������ Maze�� �ޱ� ���� ���� 
    void Start(System::Drawing::Graphics^ g);
    void Update(char direction);  // �÷��̾� �̵� ������Ʈ
    void Render(System::Drawing::Graphics^ g);  // �̷� ������ (2D ��� �׷��ִ� ��)
    // Render �Լ��� ��� ����� ȭ�� ������Ʈ
    bool IsCleared() const; // clear ���� ��ȯ 
    void SetDifficulty(int newdifficulty);

private:
    Maze* maze;
    Player* player;
    Renderer* renderer;
    int cellSize;
    bool cleared = false;
    int difficulty = 1;
};

