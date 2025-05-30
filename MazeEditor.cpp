#include "MazeEditor.h"
#include <queue>
#include <vector>

// ������
MazeEditor::MazeEditor(int size) {
    maze = new Maze(size);
    renderer = new Renderer(cellSize);
    startPoint = System::Drawing::Point(-1, -1);
    endPoint = System::Drawing::Point(-1, -1);
    hasStart = false;
    hasEnd = false;
}

// �Ҹ���: �޸� ����
MazeEditor::~MazeEditor() {
    delete maze;
    delete renderer;
}

Maze* MazeEditor::GetEditedMaze() {
    return maze; // ���۵� maze�� ���� 
}

void MazeEditor::Start() {
    // �⺻ ���� �޼��� 
    System::Windows::Forms::DialogResult result = System::Windows::Forms::MessageBox::Show(
        "��Ŭ��: Path/Wall Toggle\n��Ŭ��: ������ ����\n�� Ŭ��: ������ ����",
        "Maze Editor Instructions",
        System::Windows::Forms::MessageBoxButtons::OKCancel,
        System::Windows::Forms::MessageBoxIcon::Information
    );

    if (result != System::Windows::Forms::DialogResult::OK) return; // ��� �� 

    // Windows Forms �� ����
    editorForm = gcnew System::Windows::Forms::Form();
    editorForm->Text = "Maze Editor";
    editorForm->ClientSize = System::Drawing::Size(maze->GetSize() * cellSize, maze->GetSize() * cellSize);
    editorForm->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
    editorForm->StartPosition = System::Windows::Forms::FormStartPosition::Manual;
    editorForm->Location = System::Drawing::Point(
        (System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Width - editorForm->Width) / 2,
        (System::Windows::Forms::Screen::PrimaryScreen->WorkingArea.Height - editorForm->Height) / 2
    );

    // �̺�Ʈ �ڵ鷯 �߰�
    editorForm->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MazeEditor::HandleMouseDown);
    editorForm->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MazeEditor::DrawEditor);

    // TEST ��ư ����
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
        // �޽��� �ؽ�Ʈ ����
        System::Windows::Forms::DialogResult result = System::Windows::Forms::MessageBox::Show(
            "Valid Maze! Press Ȯ�� to try solving it.",
            "Maze Solvable",
            System::Windows::Forms::MessageBoxButtons::OK,
            System::Windows::Forms::MessageBoxIcon::Information
        );

        // ����ڰ� "Ȯ��"�� ������ �� �̷� �ذ� ���� ��ȯ
        if (result == System::Windows::Forms::DialogResult::OK) {
            solvingMode = true;
            editorForm->Close(); // ������ â �ݱ�
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
    else if (e->Button == System::Windows::Forms::MouseButtons::Right) { // ��Ŭ������ ������ ���� 
        SetStart(x, y);
    }
    else if (e->Button == System::Windows::Forms::MouseButtons::Middle) { // �� Ŭ������ ������ ����
        SetEnd(x, y);
    }

    testButton->Visible = hasStart && hasEnd;
    editorForm->Invalidate(System::Drawing::Rectangle(x * cellSize, y * cellSize, cellSize, cellSize)); // �ٲ� �� �ٽ� �׸��� 
}

void MazeEditor::SetStart(int x, int y) {
    if (hasStart && startPoint.X >= 0 && startPoint.Y >= 0) {
        editorForm->Invalidate(System::Drawing::Rectangle(startPoint.X * cellSize, startPoint.Y * cellSize, cellSize, cellSize));
        maze->SetPlaceType(startPoint.X, startPoint.Y, PATH);
    } // �������� �ٲٴ� ��� 
    maze->SetPlaceType(x, y, START);
    startPoint = System::Drawing::Point(x, y);
    hasStart = true; // start ���縦 true�� 
}

void MazeEditor::SetEnd(int x, int y) {
    if (hasEnd && endPoint.X >= 0 && endPoint.Y >= 0) {
        editorForm->Invalidate(System::Drawing::Rectangle(endPoint.X * cellSize, endPoint.Y * cellSize, cellSize, cellSize));
        maze->SetPlaceType(endPoint.X, endPoint.Y, PATH);
    } // �������� �ٲٴ� ��� 

    maze->SetPlaceType(x, y, END);
    endPoint = System::Drawing::Point(x, y);
    hasEnd = true; // end ���縦 true�� 
}

bool MazeEditor::ValidateMaze() {
    return BFS(); // BFS�� Ȱ���� �� ã�� 
}

bool MazeEditor::BFS() {
    int size = maze->GetSize();

    // �̷� ũ��� ������ visited �迭 ���� �� �ʱ�ȭ
    std::vector<std::vector<bool>> visited(size, std::vector<bool>(size, false));

    // ť�� ���� �迭 ����
    std::queue<std::pair<int, int>> q;
    int dx[] = { 0, 0, -1, 1 };
    int dy[] = { -1, 1, 0, 0 };

    // ������ ��ǥ ã�� �� ť�� �߰�
    int startX = startPoint.X;
    int startY = startPoint.Y;
    if (startX < 0 || startY < 0) return false; // �������� �������� �ʾ����� false ��ȯ

    q.push({ startX, startY });
    visited[startX][startY] = true;

    // BFS Ž�� ����
    while (!q.empty()) {
        std::pair<int, int> qf = q.front();
        q.pop();

        // ���� ��ġ�� ������(END)���� Ȯ��
        if (maze->GetPlaceType(qf.first, qf.second) == END) {
            return true;
        }

        // �����¿� ���� ���� �˻�
        for (int i = 0; i < 4; i++) {
            int nx = qf.first + dx[i];
            int ny = qf.second + dy[i];

            // ��ǥ�� ��ȿ�ϰ�, �湮���� �ʾ�����, ����� ���
            if (nx >= 0 && nx < size && ny >= 0 && ny < size &&
                !visited[nx][ny] && maze->GetPlaceType(nx, ny) != WALL) {
                visited[nx][ny] = true; // �湮 ǥ��
                q.push({ nx, ny }); // ť�� �߰�
            }
        }
    }

    // ������(END)�� �������� ���� ��� false ��ȯ
    return false;
}

bool MazeEditor::IsSolvingMode() {
    return solvingMode;
}

System::Drawing::Point MazeEditor::GetStartPosition() {
    return startPoint;
}
