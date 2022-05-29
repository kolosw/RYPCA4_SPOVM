#pragma once
#include <Windows.h>
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <stack>
#include "RegistryHandler.h"
#include <iostream>
using namespace std;
class Menu
{
private:
	RegistryHandler handler;
	stack<string> upwards;
	LSTATUS result;
	string cur;
	bool printValues;
	bool printSubkeys;
public:
	Menu()
	{
		upwards = *new stack<string>;
		handler = *new RegistryHandler();
		cur = "";
		printValues = false;
		printSubkeys = false;
	}
	HKEY chooseHkey();
	void menu(HKEY top);
	void gitMenu();
	
	BOOL SetPrivilege(
		HANDLE hToken,          // access token handle
		LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
		BOOL bEnablePrivilege   // to enable or disable privilege
	);
	
};