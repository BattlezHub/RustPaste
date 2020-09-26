#include <Windows.h>
#include "imports.h"

bool MainInit = false;
void Main() {
	if (MainInit) return;

	GetModBase(); //init g_Base

	il2cpp::InitIL();

	GUI::Init(); //AnalGUI > all
	//EnableDebugCamera();

	//const char* str = std::to_string(GetModBase()).c_str();

	//MessageBoxA(NULL, str, str, 0);

	Hooks::Init();
	MainInit = true;
}