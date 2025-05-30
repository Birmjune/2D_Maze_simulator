#include "Game.h"
#include "MazeEditor.h"

Game::Game(int cellSize, int difficulty) {
    int mazesize = 10 * difficulty + 1;
    maze = new Maze(mazesize); // ���̵� ���� �ٸ� ũ���� �̷� ����
    player = new Player(1, 1);   // �÷��̾� �ʱ� ��ġ (start ��ġ�� ����)
    renderer = new Renderer(cellSize);
}

Game::Game(Maze* editedMaze, int cellSize, System::Drawing::Point startPosition) { // ������ �̷θ� ��� ���� ��� 
    maze = editedMaze; // ������ �̷θ� �״�� ���
    player = new Player(startPosition.X, startPosition.Y); // �÷��̾� �ʱ� ��ġ ����
    renderer = new Renderer(cellSize);
}

void Game::Start(System::Drawing::Graphics^ g) {
    if (difficulty == 0) {
        MazeEditor editor(maze->GetSize());
        editor.Start();

        // ������ �̷θ� �޾ƿͼ� ���
        *maze = *editor.GetEditedMaze();
        return;
    }
    else if (difficulty < 0) {
        Render(g); // ���� �Ϸ� �� �̷θ� ������
        return;
    }
    else {  
        maze->GenerateRandomMaze();
    }

    Render(g);
}


void Game::Update(char direction) {
    if (!cleared) {
        switch (direction) {
        case 'w': player->Move(0, -1, *maze); break;
        case 's': player->Move(0, 1, *maze); break;
        case 'a': player->Move(-1, 0, *maze); break;
        case 'd': player->Move(1, 0, *maze); break;
        }

        // �̵��� ��ġ�� END������ Ȯ�� 
        if (maze->GetPlaceType(player->GetX(), player->GetY()) == END)
            cleared = true;
    }

}

void Game::Render(System::Drawing::Graphics^ g) {
    if (cleared) {
        renderer->ShowClearMessage(g);  // Ŭ���� ���¶�� �޽��� ���
    }
    else {
        renderer->Render2D(*maze, player, g);  // �̷ο� �÷��̾� ������
    }
}

bool Game::IsCleared() const {
    return cleared;
}

void Game::SetDifficulty(int newdifficulty) {
    difficulty = newdifficulty;
}