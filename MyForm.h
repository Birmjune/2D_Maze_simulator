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
            originalClientSize = this->ClientSize; // 초기 ClientSize 저장
            originalSize = this->Size;             // 초기 전체 Size 저장
        }

    protected:
        ~MyForm() {
            if (game != nullptr) {
                delete game;  // Game 객체 해제
            }
        }

    private:
        Game* game = nullptr;                       // Game 객체 초기화
        Maze* editedMaze = nullptr;
        MazeEditor^ editor = nullptr; 
        Graphics^ graphics = nullptr;               // Graphics 객체 초기화
        int cellSize = 20;                          // 셀 크기 설정
        TextBox^ difficultyInput;                   // 난이도 입력 필드
        Label^ difficultyLabel;                     // "Enter Difficulty" 텍스트 라벨
        Button^ startButton;                        // 시작 버튼
        System::Drawing::Size originalClientSize;   // 초기 ClientSize 저장용 변수
        System::Drawing::Size originalSize;         // 초기 전체 Size 저장용 변수

        void InitializeComponent() {
            this->SuspendLayout();
            this->Name = L"MyForm";
            this->Text = L"Maze Simulator";
            this->KeyPreview = true;
            this->KeyDown += gcnew KeyEventHandler(this, &MyForm::MyForm_KeyDown);
            this->ClientSize = System::Drawing::Size(420, 420);
            this->StartPosition = FormStartPosition::CenterScreen;
            this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MyForm_Paint);

            // "Enter Difficulty" 라벨 생성 및 설정
            difficultyLabel = gcnew Label();
            difficultyLabel->Text = "Enter Difficulty (0 for edit mode)";
            difficultyLabel->Font = gcnew System::Drawing::Font("Arial", 14);
            difficultyLabel->AutoSize = true;
            //difficultyLabel->Location = Point((this->ClientSize.Width / 2) - 100, (this->ClientSize.Height / 2) - 60);
            this->Controls->Add(difficultyLabel);

            // 난이도 입력 필드 생성 및 설정
            difficultyInput = gcnew TextBox();
            //difficultyInput->Location = Point((this->ClientSize.Width / 2) - 50, (this->ClientSize.Height / 2) - 30);
            difficultyInput->Size = System::Drawing::Size(100, 20);
            this->Controls->Add(difficultyInput);

            // 시작 버튼 생성 및 설정
            startButton = gcnew Button();
            startButton->Text = L"Start";
            //startButton->Location = Point((this->ClientSize.Width / 2) - 50, (this->ClientSize.Height / 2));
            startButton->Click += gcnew EventHandler(this, &MyForm::OnStartButtonClick);
            this->Controls->Add(startButton);

            Center();
            this->ResumeLayout(false);
        }

        void Center() { // 화면 중간에 버튼, 텍스트 상자 등 설치 
            difficultyLabel->Location = Point((this->ClientSize.Width - difficultyLabel->Width) / 2, (this->ClientSize.Height / 2) - 60);
            difficultyInput->Location = Point((this->ClientSize.Width - difficultyInput->Width) / 2, (this->ClientSize.Height / 2) - 30);
            startButton->Location = Point((this->ClientSize.Width - startButton->Width) / 2, (this->ClientSize.Height / 2));
        }

        void MyForm_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
            if (game != nullptr) {
                game->Render(e->Graphics); // 게임 상태를 화면에 그리기 (Paint 핸들러 설정)
            }
        }

        void StartGameWithEditedMaze() {
            if (editedMaze != nullptr) {
                // 기존 game 객체를 해제하고 새로 생성
                if (game != nullptr) {
                    delete game;
                    game = nullptr;
                }

                // 편집된 미로로 Game 객체 생성
                System::Drawing::Point startPosition = editor->GetStartPosition();
                game = new Game(editedMaze, cellSize, startPosition);
                game->SetDifficulty(-1);

                // 기존 UI 요소 숨기기
                difficultyLabel->Visible = false;
                difficultyInput->Visible = false;
                startButton->Visible = false;

                // 화면을 지우고 새롭게 렌더링
                graphics = CreateGraphics();
                graphics->Clear(System::Drawing::Color::Black); // 배경을 검은색으로 초기화
                game->Start(graphics);

                this->Invalidate(); // 화면 렌더링 문제 해결 위해 Paint()를 새로 지정 
            }
        }

        // Start 버튼 누르는 경우 
        void OnStartButtonClick(Object^ sender, EventArgs^ e) {
            try {
                int difficulty = Int32::Parse(difficultyInput->Text);

                if (difficulty == 0) {
                    // 편집 모드 실행 시 현재 창 숨기기
                    this->Hide();

                    // 편집기 실행
                    editor = gcnew MazeEditor(21); // 21x21 크기의 편집기 생성
                    editor->Start();

                    // 편집이 끝난 후 Solve 모드로 전환 여부 확인
                    if (editor->IsSolvingMode()) {
                        editedMaze = editor->GetEditedMaze();
                        StartGameWithEditedMaze();
                        this->Show();
                        return;
                    }

                    // 편집 모드 종료 후 폼 다시 표시
                    this->Show();
                    ResetGame();
                    return;
                }

                // 난이도가 0이 아닐 경우, 기존 게임 실행 로직
                int mazeSize = (10 * difficulty + 1) * cellSize;

                // 창 크기 변경
                this->ClientSize = System::Drawing::Size(mazeSize, mazeSize);
                this->StartPosition = FormStartPosition::Manual;
                this->Location = Point(
                    (Screen::PrimaryScreen->WorkingArea.Width - this->Width) / 2,
                    (Screen::PrimaryScreen->WorkingArea.Height - this->Height) / 2
                );

                // 난이도 입력 관련 컨트롤 숨기기
                difficultyLabel->Visible = false;
                difficultyInput->Visible = false;
                startButton->Visible = false;

                // 이전 게임 객체 삭제 후 새로운 Game 객체 생성
                if (game != nullptr) {
                    delete game;
                    game = nullptr;
                }
                game = new Game(cellSize, difficulty);
                graphics = CreateGraphics();
                if (graphics != nullptr)
                    graphics->Clear(System::Drawing::Color::White);

                game->Start(graphics); // 게임 시작 및 미로 렌더링
            }

            catch (FormatException^) {
                MessageBox::Show("Please enter a valid difficulty level.", "Input Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }


        void MyForm_KeyDown(Object^ sender, KeyEventArgs^ e) {
            if (game != nullptr) {  // 게임이 시작된 경우에만 키 입력 처리
                // 클리어 상태에서 'R' 키가 눌리면 초기화
                if (game->IsCleared() && e->KeyCode == Keys::R) {
                    ResetGame();  // 초기화 함수 호출
                }
                else {
                    // 게임 중 키 입력 처리
                    switch (e->KeyCode) {
                    case Keys::W: game->Update('w'); break;
                    case Keys::S: game->Update('s'); break;
                    case Keys::A: game->Update('a'); break;
                    case Keys::D: game->Update('d'); break;
                    }
                    game->Render(graphics);  // 이동 후 화면 갱신
                }
            }
        }

        // 게임 초기화 함수
        void ResetGame() {
            // 기존 게임 객체 삭제
            if (game != nullptr) {
                delete game;
                game = nullptr;
            }

            // 창 크기를 초기 크기로 복원
            this->Size = originalSize; // 전체 크기 복원
            this->ClientSize = originalClientSize; // Client 크기 복원

            // 창을 화면의 가운데로 배치
            this->StartPosition = FormStartPosition::Manual;
            this->Location = Point(
                (Screen::PrimaryScreen->WorkingArea.Width - this->Width) / 2,
                (Screen::PrimaryScreen->WorkingArea.Height - this->Height) / 2
            );

            // 화면 초기화
            graphics = CreateGraphics();
            graphics->Clear(System::Drawing::Color::White);

            // 난이도 입력 창과 버튼을 다시 화면 중앙에 배치
            Center();

            // 난이도 입력 창과 버튼을 다시 표시
            difficultyLabel->Visible = true;
            difficultyInput->Visible = true;
            startButton->Visible = true;
        }
    };
}