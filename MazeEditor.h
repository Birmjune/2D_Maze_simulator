#pragma once
#include "Maze.h"
#include "Renderer.h"
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

public ref class MazeEditor {
public:
    MazeEditor(int size);
    ~MazeEditor(); // 소멸자
    void Start(); // 편집 모드 시작
    Maze* GetEditedMaze(); // 네이티브 포인터 반환
    bool ValidateMaze(); // 미로 유효성 검사
    bool IsSolvingMode(); // solve 모드 여부 확인 
    System::Drawing::Point GetStartPosition();

private:
    Maze* maze;
    Renderer* renderer;
    int cellSize = 20;
    bool hasStart = false;
    bool hasEnd = false;
    bool solvingMode = false;
    System::Drawing::Point startPoint;
    System::Drawing::Point endPoint;

    // Windows Forms 요소
    System::Windows::Forms::Form^ editorForm;
    System::Windows::Forms::Button^ testButton;
    System::Drawing::Graphics^ graphics;

    // 이벤트 핸들러와 그리기 함수
    void DrawEditor(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
    void HandleMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
    void HandleTestClick(System::Object^ sender, System::EventArgs^ e);

    // 내부 함수
    void SetStart(int x, int y);
    void SetEnd(int x, int y);
    bool BFS(); // BFS를 사용한 경로 탐색
};
