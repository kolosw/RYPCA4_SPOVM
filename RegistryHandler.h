#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
using namespace std;
class RegistryHandler
{
private:
	LSTATUS result;
	BYTE databuffer[256];
public:
	LSTATUS RestoreRegistry(HKEY RootKey, string filename);
	LSTATUS SaveRegistry(HKEY RootKey, string filename);
	LSTATUS EditValue(HKEY RootKey, const char* subKey, const char* valueName);
	LSTATUS PrintValues(HKEY hKey, DWORD numValues, int tabs, string subKey);
	LSTATUS PrintSubKeys(HKEY RootKey, const char* subKey, bool flagSub, bool flagValues, unsigned int tabs);
	LSTATUS addValue(HKEY top, string cur);
	LSTATUS deleteValue(HKEY top, const char* subkey, const char* valuename);
	LSTATUS deleteSubkey(HKEY top, const char* subkey);
};