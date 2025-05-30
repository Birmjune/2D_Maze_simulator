#pragma once
#include "Maze.h"
#include "Player.h"
#using <System.Drawing.dll>

class Renderer {
public:
    Renderer(int cellSize);
    void Render2D(const Maze& maze, const Player* player, System::Drawing::Graphics^ g); // Maze를 깨는 모습을 form에 그려주는 함수 
    void ShowClearMessage(System::Drawing::Graphics^ g); // 미로를 깨면 clear 메세지 생성

private:
    int cellSize;  // 셀 크기 (하나의 칸의 크기, 그림그리는 픽셀 수)
};
