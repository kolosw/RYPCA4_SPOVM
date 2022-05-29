#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <stack>
#include "Menu.h"
#include "RegistryHandler.h"
using namespace std;


int main(int argc,char* argv)
{
	_wmkdir(L"registers");
	Menu men = *new Menu();
	HANDLE ProcessToken;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &ProcessToken)) {
		if (men.SetPrivilege(ProcessToken, SE_BACKUP_NAME , TRUE) == FALSE)
			return -1;
		if (men.SetPrivilege(ProcessToken, SE_RESTORE_NAME, TRUE) == FALSE)
			return -1;
	}
	men.menu(men.chooseHkey());
	return 0;
}