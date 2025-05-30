#include "Renderer.h"

Renderer::Renderer(int cellSize) : cellSize(cellSize) {} // ������ 

void Renderer::Render2D(const Maze& maze, const Player* player, System::Drawing::Graphics^ g) {
    int mazeSize = maze.GetSize();

    for (int y = 0; y < mazeSize; y++) {
        for (int x = 0; x < mazeSize; x++) {
            System::Drawing::Rectangle cellRect(x * cellSize, y * cellSize, cellSize, cellSize);

            // �÷��̾ nullptr�� �ƴϰ�, ���� ��ǥ�� �÷��̾ �ִ� ��� �÷��̾ �׸���
            if (player != nullptr && x == player->GetX() && y == player->GetY()) {
                // �÷��̾� ��ġ�� �� ũ���� 2/3�� �ٿ��� (�ֺ��� ���̰�)
                int left = cellSize / 6;
                System::Drawing::Rectangle playerRect(x * cellSize + left, y * cellSize + left, cellSize * 2 / 3, cellSize * 2 / 3);
                g->FillRectangle(System::Drawing::Brushes::Red, playerRect); // �÷��̾� ��ġ
            }
            else if (maze.GetPlaceType(x, y) == WALL) {
                g->FillRectangle(System::Drawing::Brushes::Black, cellRect); // ��
            }
            else if (maze.GetPlaceType(x, y) == PATH) {
                g->FillRectangle(System::Drawing::Brushes::White, cellRect); // �� ĭ (�̵� ���� ���)
            }
            else if (maze.GetPlaceType(x, y) == START) {
                g->FillRectangle(System::Drawing::Brushes::Green, cellRect); // ������
            }
            else if (maze.GetPlaceType(x, y) == END) {
                g->FillRectangle(System::Drawing::Brushes::Blue, cellRect); // ������
            }

            // �� �׵θ� �׸��� (���� ��忡�� �� �� ���̵���) 
            if (player == nullptr)
                g->DrawRectangle(System::Drawing::Pens::Gray, cellRect);
        }
    }
}

void Renderer::ShowClearMessage(System::Drawing::Graphics^ g) {
    g->Clear(System::Drawing::Color::White); // ȭ���� ������� 
    System::Drawing::Font^ font = gcnew System::Drawing::Font("Arial", 24);
    System::Drawing::StringFormat^ format = gcnew System::Drawing::StringFormat();
    format->Alignment = System::Drawing::StringAlignment::Center;
    format->LineAlignment = System::Drawing::StringAlignment::Center;

    System::Drawing::RectangleF rect = g->VisibleClipBounds;

    // "You cleared!" �޽��� ǥ��
    rect.Y -= 20;
    g->DrawString("You cleared!", font, System::Drawing::Brushes::Red, rect, format);

    // "Press [R] to retry" �޽��� ǥ�� (�Ʒ��� ��ġ)
    System::Drawing::Font^ smallFont = gcnew System::Drawing::Font("Arial", 16);
    rect.Y += 40;  // �Ʒ������� �ణ �̵�
    g->DrawString("Press [R] to retry", smallFont, System::Drawing::Brushes::Black, rect, format);
}


