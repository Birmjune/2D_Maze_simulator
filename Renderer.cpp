#include "Renderer.h"

Renderer::Renderer(int cellSize) : cellSize(cellSize) {} // 생성자 

void Renderer::Render2D(const Maze& maze, const Player* player, System::Drawing::Graphics^ g) {
    int mazeSize = maze.GetSize();

    for (int y = 0; y < mazeSize; y++) {
        for (int x = 0; x < mazeSize; x++) {
            System::Drawing::Rectangle cellRect(x * cellSize, y * cellSize, cellSize, cellSize);

            // 플레이어가 nullptr가 아니고, 현재 좌표에 플레이어가 있는 경우 플레이어를 그리기
            if (player != nullptr && x == player->GetX() && y == player->GetY()) {
                // 플레이어 위치를 셀 크기의 2/3로 줄여서 (주변이 보이게)
                int left = cellSize / 6;
                System::Drawing::Rectangle playerRect(x * cellSize + left, y * cellSize + left, cellSize * 2 / 3, cellSize * 2 / 3);
                g->FillRectangle(System::Drawing::Brushes::Red, playerRect); // 플레이어 위치
            }
            else if (maze.GetPlaceType(x, y) == WALL) {
                g->FillRectangle(System::Drawing::Brushes::Black, cellRect); // 벽
            }
            else if (maze.GetPlaceType(x, y) == PATH) {
                g->FillRectangle(System::Drawing::Brushes::White, cellRect); // 빈 칸 (이동 가능 경로)
            }
            else if (maze.GetPlaceType(x, y) == START) {
                g->FillRectangle(System::Drawing::Brushes::Green, cellRect); // 시작점
            }
            else if (maze.GetPlaceType(x, y) == END) {
                g->FillRectangle(System::Drawing::Brushes::Blue, cellRect); // 도착점
            }

            // 셀 테두리 그리기 (편집 모드에서 더 잘 보이도록) 
            if (player == nullptr)
                g->DrawRectangle(System::Drawing::Pens::Gray, cellRect);
        }
    }
}

void Renderer::ShowClearMessage(System::Drawing::Graphics^ g) {
    g->Clear(System::Drawing::Color::White); // 화면을 흰색으로 
    System::Drawing::Font^ font = gcnew System::Drawing::Font("Arial", 24);
    System::Drawing::StringFormat^ format = gcnew System::Drawing::StringFormat();
    format->Alignment = System::Drawing::StringAlignment::Center;
    format->LineAlignment = System::Drawing::StringAlignment::Center;

    System::Drawing::RectangleF rect = g->VisibleClipBounds;

    // "You cleared!" 메시지 표시
    rect.Y -= 20;
    g->DrawString("You cleared!", font, System::Drawing::Brushes::Red, rect, format);

    // "Press [R] to retry" 메시지 표시 (아래쪽 위치)
    System::Drawing::Font^ smallFont = gcnew System::Drawing::Font("Arial", 16);
    rect.Y += 40;  // 아래쪽으로 약간 이동
    g->DrawString("Press [R] to retry", smallFont, System::Drawing::Brushes::Black, rect, format);
}


