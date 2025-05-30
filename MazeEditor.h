#pragma once
#include "Maze.h"
#include "Renderer.h"
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

public ref class MazeEditor {
public:
    MazeEditor(int size);
    ~MazeEditor(); // �Ҹ���
    void Start(); // ���� ��� ����
    Maze* GetEditedMaze(); // ����Ƽ�� ������ ��ȯ
    bool ValidateMaze(); // �̷� ��ȿ�� �˻�
    bool IsSolvingMode(); // solve ��� ���� Ȯ�� 
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

    // Windows Forms ���
    System::Windows::Forms::Form^ editorForm;
    System::Windows::Forms::Button^ testButton;
    System::Drawing::Graphics^ graphics;

    // �̺�Ʈ �ڵ鷯�� �׸��� �Լ�
    void DrawEditor(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e);
    void HandleMouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
    void HandleTestClick(System::Object^ sender, System::EventArgs^ e);

    // ���� �Լ�
    void SetStart(int x, int y);
    void SetEnd(int x, int y);
    bool BFS(); // BFS�� ����� ��� Ž��
};
