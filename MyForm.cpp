#include "MyForm.h"

#include "MyForm.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace MazeSimulator; //form1: ������Ʈ �̸�
[STAThreadAttribute]
int main(array<System::String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew MyForm); //MyForm: Windows Form ������� �̸�
	return 0;
}