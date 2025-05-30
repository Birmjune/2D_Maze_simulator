#include "MazeEditor.h"
#include <queue>
#include <vector>

// 생성자
MazeEditor::MazeEditor(int size) {
    maze = new Maze(size);
    renderer = new Renderer(cellSize);
    startPoint = System::Drawing::Point(-1, -1);
    endPoint = System::Drawing::Point(-1, -1);
    hasStart = false;
    hasEnd = false;
}

// 소멸자: 메모리 해제
MazeEditor::~MazeEditor() {
    delete maze;
    delete renderer;
}

Maze* MazeEditor::GetEditedMaze() {
    return maze; // 제작된 maze를 리턴 
}

void MazeEditor::Start() {
    // 기본 조작 메세지 
    System::Windows::Forms::DialogResult result = System::Windows::Forms::MessageBox::Show(
        "좌클릭: Path/Wall Toggle\n우클릭: 시작점 설정\n휠 클릭: 도착점 설정",
        "Maze Editor Instructions",
        System::Windows::Forms::MessageBoxButtons::OKCancel,
        System::Windows::Forms::MessageBoxIcon::Information
    );

    if (result != System::Windows::Forms::DialogResult::OK) return; // 취소 시 

    // Windows Forms 폼 생성
    editorForm = gcnew System::Windows::Forms::Form();
    editorForm->Text = "Maze Editor";
    editorForm->ClientSize = System::Drawing::Size(maze->GetSize() * cellSize, maze->GetSize() * cellSize);
    editorForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
    editorForm->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
    editorForm->Location = System::Drawing::Point(
        (System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Width - editorForm->Width) / 2,
        (System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Height - editorForm->Height) / 2
    );

    // 이벤트 핸들러 추가
    editorForm->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MazeEditor::HandleMouseDown);
    editorForm->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MazeEditor::DrawEditor);

    // TEST 버튼 생성
    testButton = gcnew System::Windows::Forms::Button();
    testButton->Text = "TEST";
    testButton->Visible = false;
    testButton->Location = System::Drawing::Point(editorForm->ClientSize.Width - 80, editorForm->ClientSize.Height - 40);
    testButton->Click += gcnew System::EventHandler(this, &MazeEditor::HandleTestClick);
    editorForm->Controls->Add(testButton);

    editorForm->ShowDialog();
}

void MazeEditor::DrawEditor(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
    renderer->Render2D(*maze, nullptr, e->Graphics);
}

void MazeEditor::HandleTestClick(System::Object^ sender, System::EventArgs^ e) {
    if (ValidateMaze()) {
        // 메시지 텍스트 수정
        System::Windows::Forms::DialogResult result = System::Windows::Forms::MessageBox::Show(
            "Valid Maze! Press 확인 to try solving it.",
            "Maze Solvable",
            System::Windows::Forms::MessageBoxButtons::OK,
            System::Windows::Forms::MessageBoxIcon::Information
        );

        // 사용자가 "확인"을 눌렀을 때 미로 해결 모드로 전환
        if (result == System::Windows::Forms::DialogResult::OK) {
            solvingMode = true;
            editorForm->Close(); // 편집기 창 닫기
        }
    }
    else {
        System::Windows::Forms::MessageBox::Show("Maze has no valid path!", "Error");
    }
}



void MazeEditor::HandleMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
    int x = e->X / cellSize;
    int y = e->Y / cellSize;

    if (x < 0 || y < 0 || x >= maze->GetSize() || y >= maze->GetSize()) return;

    if (e->Button == System::Windows::Forms::MouseButtons::Left) {
        maze->SetPlaceType(x, y, maze->GetPlaceType(x, y) == WALL ? PATH : WALL);
    }
    else if (e->Button == System::Windows::Forms::MouseButtons::Right) { // 우클릭으로 시작점 설정 
        SetStart(x, y);
    }
    else if (e->Button == System::Windows::Forms::MouseButtons::Middle) { // 휠 클릭으로 종료점 설정
        SetEnd(x, y);
    }

    testButton->Visible = hasStart && hasEnd;
    editorForm->Invalidate(System::Drawing::Rectangle(x * cellSize, y * cellSize, cellSize, cellSize)); // 바뀐 셀 다시 그리기 
}

void MazeEditor::SetStart(int x, int y) {
    if (hasStart && startPoint.X >= 0 && startPoint.Y >= 0) {
        editorForm->Invalidate(System::Drawing::Rectangle(startPoint.X * cellSize, startPoint.Y * cellSize, cellSize, cellSize));
        maze->SetPlaceType(startPoint.X, startPoint.Y, PATH);
    } // 시작점을 바꾸는 경우 
    maze->SetPlaceType(x, y, START);
    startPoint = System::Drawing::Point(x, y);
    hasStart = true; // start 존재를 true로 
}

void MazeEditor::SetEnd(int x, int y) {
    if (hasEnd && endPoint.X >= 0 && endPoint.Y >= 0) {
        editorForm->Invalidate(System::Drawing::Rectangle(endPoint.X * cellSize, endPoint.Y * cellSize, cellSize, cellSize));
        maze->SetPlaceType(endPoint.X, endPoint.Y, PATH);
    } // 도착점을 바꾸는 경우 

    maze->SetPlaceType(x, y, END);
    endPoint = System::Drawing::Point(x, y);
    hasEnd = true; // end 존재를 true로 
}

bool MazeEditor::ValidateMaze() {
    return BFS(); // BFS를 활용해 길 찾기 
}

bool MazeEditor::BFS() {
    int size = maze->GetSize();

    // 미로 크기와 동일한 visited 배열 생성 및 초기화
    std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));

    // 큐와 방향 배열 정의
    std::queue<std::pair<int, int>> q;
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    // 시작점 좌표 찾기 및 큐에 추가
    int startX = startPoint.X;
    int startY = startPoint.Y;
    if (startX < 0 || startY < 0) return false; // 시작점이 설정되지 않았으면 false 반환

    q.push({ startX, startY });
    visited[startX][startY] = true;

    // BFS 탐색 시작
    while (!q.empty()) {
        std::pair<int, int> qf = q.front();
        q.pop();

        // 현재 위치가 도착점(END)인지 확인
        if (maze->GetPlaceType(qf.first, qf.second) == END) {
            return true;
        }

        // 상하좌우 인접 셀을 검사
        for (int i = 0; i < 4; i++) {
            int nx = qf.first + dx[i];
            int ny = qf.second + dy[i];

            // 좌표가 유효하고, 방문하지 않았으며, 경로인 경우
            if (nx >= 0 && nx < size && ny >= 0 && ny < size &&
                !visited[nx][ny] && maze->GetPlaceType(nx, ny) != WALL) {
                visited[nx][ny] = true; // 방문 표시
                q.push({ nx, ny }); // 큐에 추가
            }
        }
    }

    // 도착점(END)에 도달하지 못한 경우 false 반환
    return false;
}

bool MazeEditor::IsSolvingMode() {
    return solvingMode;
}

System::Drawing::Point MazeEditor::GetStartPosition() {
    return startPoint;
}
