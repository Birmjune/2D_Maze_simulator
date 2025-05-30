#pragma once
#include "Game.h"
#include "MazeEditor.h"
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

namespace MazeSimulator {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Drawing;

    public ref class MyForm : public Form {
    public:
        MyForm() {
            InitializeComponent();
            originalClientSize = this->ClientSize; // �ʱ� ClientSize ����
            originalSize = this->Size;             // �ʱ� ��ü Size ����
        }

    protected:
        ~MyForm() {
            if (game != nullptr) {
                delete game;  // Game ��ü ����
            }
        }

    private:
        Game* game = nullptr;                       // Game ��ü �ʱ�ȭ
        Maze* editedMaze = nullptr;
        MazeEditor^ editor = nullptr; 
        Graphics^ graphics = nullptr;               // Graphics ��ü �ʱ�ȭ
        int cellSize = 20;                          // �� ũ�� ����
        TextBox^ difficultyInput;                   // ���̵� �Է� �ʵ�
        Label^ difficultyLabel;                     // "Enter Difficulty" �ؽ�Ʈ ��
        Button^ startButton;                        // ���� ��ư
        System::Drawing::Size originalClientSize;   // �ʱ� ClientSize ����� ����
        System::Drawing::Size originalSize;         // �ʱ� ��ü Size ����� ����

        void InitializeComponent() {
            this->SuspendLayout();
            this->Name = L"MyForm";
            this->Text = L"Maze Simulator";
            this->KeyPreview = true;
            this->KeyDown += gcnew KeyEventHandler(this, &MyForm::MyForm_KeyDown);
            this->ClientSize = System::Drawing::Size(420, 420);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MyForm_Paint);

            // "Enter Difficulty" �� ���� �� ����
            difficultyLabel = gcnew Label();
            difficultyLabel->Text = "Enter Difficulty (0 for edit mode)";
            difficultyLabel->Font = gcnew System::Drawing::Font("Arial", 14);
            difficultyLabel->AutoSize = true;
            //difficultyLabel->Location = Point((this->ClientSize.Width / 2) - 100, (this->ClientSize.Height / 2) - 60);
            this->Controls->Add(difficultyLabel);

            // ���̵� �Է� �ʵ� ���� �� ����
            difficultyInput = gcnew TextBox();
            //difficultyInput->Location = Point((this->ClientSize.Width / 2) - 50, (this->ClientSize.Height / 2) - 30);
            difficultyInput->Size = System::Drawing::Size(100, 20);
            this->Controls->Add(difficultyInput);

            // ���� ��ư ���� �� ����
            startButton = gcnew Button();
            startButton->Text = L"Start";
            //startButton->Location = Point((this->ClientSize.Width / 2) - 50, (this->ClientSize.Height / 2));
            startButton->Click += gcnew EventHandler(this, &MyForm::OnStartButtonClick);
            this->Controls->Add(startButton);

            Center();
            this->ResumeLayout(false);
        }

        void Center() { // ȭ�� �߰��� ��ư, �ؽ�Ʈ ���� �� ��ġ 
            difficultyLabel->Location = Point((this->ClientSize.Width - difficultyLabel->Width) / 2, (this->ClientSize.Height / 2) - 60);
            difficultyInput->Location = Point((this->ClientSize.Width - difficultyInput->Width) / 2, (this->ClientSize.Height / 2) - 30);
            startButton->Location = Point((this->ClientSize.Width - startButton->Width) / 2, (this->ClientSize.Height / 2));
        }

        void MyForm_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
            if (game != nullptr) {
                game->Render(e->Graphics); // ���� ���¸� ȭ�鿡 �׸��� (Paint �ڵ鷯 ����)
            }
        }

        void StartGameWithEditedMaze() {
            if (editedMaze != nullptr) {
                // ���� game ��ü�� �����ϰ� ���� ����
                if (game != nullptr) {
                    delete game;
                    game = nullptr;
                }

                // ������ �̷η� Game ��ü ����
                System::Drawing::Point startPosition = editor->GetStartPosition();
                game = new Game(editedMaze, cellSize, startPosition);
                game->SetDifficulty(-1);

                // ���� UI ��� �����
                difficultyLabel->Visible = false;
                difficultyInput->Visible = false;
                startButton->Visible = false;

                // ȭ���� ����� ���Ӱ� ������
                graphics = CreateGraphics();
                graphics->Clear(System::Drawing::Color::Black); // ����� ���������� �ʱ�ȭ
                game->Start(graphics);

                this->Invalidate(); // ȭ�� ������ ���� �ذ� ���� Paint()�� ���� ���� 
            }
        }

        // Start ��ư ������ ��� 
        void OnStartButtonClick(Object^ sender, EventArgs^ e) {
            try {
                int difficulty = Int32::Parse(difficultyInput->Text);

                if (difficulty == 0) {
                    // ���� ��� ���� �� ���� â �����
                    this->Hide();

                    // ������ ����
                    editor = gcnew MazeEditor(21); // 21x21 ũ���� ������ ����
                    editor->Start();

                    // ������ ���� �� Solve ���� ��ȯ ���� Ȯ��
                    if (editor->IsSolvingMode()) {
                        editedMaze = editor->GetEditedMaze();
                        StartGameWithEditedMaze();
                        this->Show();
                        return;
                    }

                    // ���� ��� ���� �� �� �ٽ� ǥ��
                    this->Show();
                    ResetGame();
                    return;
                }

                // ���̵��� 0�� �ƴ� ���, ���� ���� ���� ����
                int mazeSize = (10 * difficulty + 1) * cellSize;

                // â ũ�� ����
                this->ClientSize = System::Drawing::Size(mazeSize, mazeSize);
                this->StartPosition = FormStartPosition::Manual;
                this->Location = Point(
                    (Screen::PrimaryScreen->WorkingArea.Width - this->Width) / 2,
                    (Screen::PrimaryScreen->WorkingArea.Height - this->Height) / 2
                );

                // ���̵� �Է� ���� ��Ʈ�� �����
                difficultyLabel->Visible = false;
                difficultyInput->Visible = false;
                startButton->Visible = false;

                // ���� ���� ��ü ���� �� ���ο� Game ��ü ����
                if (game != nullptr) {
                    delete game;
                    game = nullptr;
                }
                game = new Game(cellSize, difficulty);
                graphics = CreateGraphics();
                if (graphics != nullptr)
                    graphics->Clear(System::Drawing::Color::White);

                game->Start(graphics); // ���� ���� �� �̷� ������
            }

            catch (FormatException^) {
                MessageBox::Show("Please enter a valid difficulty level.", "Input Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }


        void MyForm_KeyDown(Object^ sender, KeyEventArgs^ e) {
            if (game != nullptr) {  // ������ ���۵� ��쿡�� Ű �Է� ó��
                // Ŭ���� ���¿��� 'R' Ű�� ������ �ʱ�ȭ
                if (game->IsCleared() && e->KeyCode == Keys::R) {
                    ResetGame();  // �ʱ�ȭ �Լ� ȣ��
                }
                else {
                    // ���� �� Ű �Է� ó��
                    switch (e->KeyCode) {
                    case Keys::W: game->Update('w'); break;
                    case Keys::S: game->Update('s'); break;
                    case Keys::A: game->Update('a'); break;
                    case Keys::D: game->Update('d'); break;
                    }
                    game->Render(graphics);  // �̵� �� ȭ�� ����
                }
            }
        }

        // ���� �ʱ�ȭ �Լ�
        void ResetGame() {
            // ���� ���� ��ü ����
            if (game != nullptr) {
                delete game;
                game = nullptr;
            }

            // â ũ�⸦ �ʱ� ũ��� ����
            this->Size = originalSize; // ��ü ũ�� ����
            this->ClientSize = originalClientSize; // Client ũ�� ����

            // â�� ȭ���� ����� ��ġ
            this->StartPosition = FormStartPosition::Manual;
            this->Location = Point(
                (Screen::PrimaryScreen->WorkingArea.Width - this->Width) / 2,
                (Screen::PrimaryScreen->WorkingArea.Height - this->Height) / 2
            );

            // ȭ�� �ʱ�ȭ
            graphics = CreateGraphics();
            graphics->Clear(System::Drawing::Color::White);

            // ���̵� �Է� â�� ��ư�� �ٽ� ȭ�� �߾ӿ� ��ġ
            Center();

            // ���̵� �Է� â�� ��ư�� �ٽ� ǥ��
            difficultyLabel->Visible = true;
            difficultyInput->Visible = true;
            startButton->Visible = true;
        }
    };
}