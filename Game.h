#pragma once
#include "Maze.h"
#include "Player.h"
#include "Renderer.h"
#using <System.Drawing.dll>

class Game {
public:
    Game(int cellSize, int difficulty); 
    Game(Maze* editedMaze, int cellSize, System::Drawing::Point startPosition); // 편집된 Maze를 받기 위한 역할 
    void Start(System::Drawing::Graphics^ g);
    void Update(char direction);  // 플레이어 이동 업데이트
    void Render(System::Drawing::Graphics^ g);  // 미로 렌더링 (2D 모드 그려주는 것)
    // Render 함수를 계속 사용해 화면 업데이트
    bool IsCleared() const; // clear 유뮤 반환 
    void SetDifficulty(int newdifficulty);

private:
    Maze* maze;
    Player* player;
    Renderer* renderer;
    int cellSize;
    bool cleared = false;
    int difficulty = 1;
};

