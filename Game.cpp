#include "Game.h"
#include "MazeEditor.h"

Game::Game(int cellSize, int difficulty) {
    int mazesize = 10 * difficulty + 1;
    maze = new Maze(mazesize); // 난이도 따라 다른 크기의 미로 생성
    player = new Player(1, 1);   // 플레이어 초기 위치 (start 위치와 동일)
    renderer = new Renderer(cellSize);
}

Game::Game(Maze* editedMaze, int cellSize, System::Drawing::Point startPosition) { // 편집된 미로를 들고 오는 경우 
    maze = editedMaze; // 편집된 미로를 그대로 사용
    player = new Player(startPosition.X, startPosition.Y); // 플레이어 초기 위치 설정
    renderer = new Renderer(cellSize);
}

void Game::Start(System::Drawing::Graphics^ g) {
    if (difficulty == 0) {
        MazeEditor editor(maze->GetSize());
        editor.Start();

        // 편집된 미로를 받아와서 사용
        *maze = *editor.GetEditedMaze();
        return;
    }
    else if (difficulty < 0) {
        Render(g); // 편집 완료 후 미로를 렌더링
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

        // 이동한 위치가 END인지를 확인 
        if (maze->GetPlaceType(player->GetX(), player->GetY()) == END)
            cleared = true;
    }

}

void Game::Render(System::Drawing::Graphics^ g) {
    if (cleared) {
        renderer->ShowClearMessage(g);  // 클리어 상태라면 메시지 출력
    }
    else {
        renderer->Render2D(*maze, player, g);  // 미로와 플레이어 렌더링
    }
}

bool Game::IsCleared() const {
    return cleared;
}

void Game::SetDifficulty(int newdifficulty) {
    difficulty = newdifficulty;
}