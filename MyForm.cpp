#include "MyForm.h"

#include "MyForm.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace MazeSimulator; //form1: 프로젝트 이름
[STAThreadAttribute]
int main(array<System::String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm); //MyForm: Windows Form 헤더파일 이름
	return 0;
}