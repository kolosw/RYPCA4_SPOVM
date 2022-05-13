#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <string>
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <stack>
using namespace std;
LSTATUS result;
BYTE databuffer[256];
void PrintSubKeys(HKEY RootKey, const char* subKey, bool flagSub, bool flagValues, unsigned int tabs = 0);
void PrintValues(HKEY hKey, DWORD numValues, int tabs, string subKey);
void gitMenu();
BOOL SetPrivilege(HANDLE hToken,LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
void RestoreRegistry(HKEY RootKey, string filename);
void SaveRegistry(HKEY RootKey, string filename);
void EditValue(HKEY RootKey, const char* subKey, const char* valueName);
void menu();
void RestoreRegistry(HKEY RootKey, string filename)
{
	RegRestoreKeyA(RootKey, filename.c_str(), NULL);
}
void SaveRegistry(HKEY RootKey, string filename)
{
	RegSaveKeyA(RootKey, filename.c_str(), NULL);
}
void EditValue(HKEY RootKey, const char* subKey, const char* valueName)
{
	HKEY hKey;
	DWORD datatype;
	DWORD dataSize = 1024;
	BYTE* data;
	result = RegOpenKeyExA(RootKey, subKey, 0, KEY_ALL_ACCESS, &hKey);
	if (result != ERROR_SUCCESS) {
		cout << "\nError RegOpenKeyEx > " << result;
		RegCloseKey(hKey);
		return;
	}
	result = RegGetValueA(RootKey, subKey, valueName, NULL, &datatype, &data, &dataSize);
	if (result != ERROR_SUCCESS) {
		cout << "\nError RegOpenKeyEx > " << result;
		RegCloseKey(hKey);
		return;
	}
	if (datatype == REG_DWORD || datatype == REG_NONE) {
		int a;
		cin >> a;
		RegSetValueExA(hKey, valueName, NULL, datatype, (BYTE*)a, sizeof(a));
	}
	else if (datatype == REG_SZ || datatype == REG_LINK)
	{
		string a;
		cin >> a;
		RegSetValueExA(hKey, valueName, NULL, datatype, (BYTE*)a.c_str(), a.size());
	}
	else if (dataSize == 0)
	{
		//Can`t deal with bitsized values
	}
	RegCloseKey(hKey);
}
void PrintValues(HKEY hKey, DWORD numValues, int tabs, string subKey)
{
	DWORD dwIndex = 0;
	LPSTR valueName = new CHAR[256];
	DWORD valNameLen;
	BYTE* data = databuffer;
	DWORD dataSize = 1024;
	DWORD datatype;
	for (int i = 0; i < numValues; i++)
	{
		DWORD valNameLen = 256;
		dataSize = 256;
		result =
			RegEnumValueA(hKey,
				dwIndex,
				valueName,
				&valNameLen,
				NULL,
				&datatype,
				data,
				&dataSize);

		if (result != ERROR_SUCCESS) {
			cout << "\nError RegEnumValue > " << result << endl;
			//RegCloseKey(hKey);
			return;
		}
		if (datatype == REG_DWORD || datatype == REG_NONE) {
			int a = *data;
			for (int i = 0; i < tabs; i++)
				printf("\t");
			cout << "Value name: " << valueName<<endl;
			for (int i = 0; i < tabs; i++)
				printf("\t");
			cout << "Value size: " << dataSize << endl;
			for (int i = 0; i < tabs; i++)
				printf("\t");
			cout << "Value data: " << a << endl;
		}
		else if (dataSize == 0)
		{
			for (int i = 0; i < tabs; i++)
				printf("\t");
			cout << "Value name: " << valueName<<endl;
		}
		else
		{
			for (int i = 0; i < tabs; i++)
				printf("\t");
			cout << "Value name: " << valueName;
			cout << endl;
			for (int i = 0; i < tabs; i++)
				printf("\t");
			cout << "Value size: " << dataSize << endl;
			for (int i = 0; i < tabs; i++)
				printf("\t");
			cout << "Value data: " << data << endl;
		}
		//cout << "\nValue size: " << dataSize << "\nValue data: " << data;
		dwIndex++;
	}
	//RegCloseKey(hKey);
}

void PrintSubKeys(HKEY RootKey, const char* subKey, bool flagSub, bool flagValues, unsigned int tabs)
{
	HKEY hKey;
	DWORD cSubKeys;        //Used to store the number of Subkeys
	DWORD maxSubkeyLen;    //Longest Subkey name length
	DWORD cValues;        //Used to store the number of Subkeys
	DWORD maxValueLen;    //Longest Subkey name length
	DWORD retCode;        //Return values of calls

	result =
		RegOpenKeyExA(RootKey, subKey, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS | KEY_READ /*KEY_ALL_ACCESS*/, &hKey);
	if (result != ERROR_SUCCESS) {
		cout << "\nError RegOpenKeyEx > " << result;
		RegCloseKey(hKey);
		return;
	}

	result =
		RegQueryInfoKeyA(hKey,          // key handle
			NULL,            // buffer for class name
			NULL,            // size of class string
			NULL,            // reserved
			&cSubKeys,        // number of subkeys
			&maxSubkeyLen,    // longest subkey length
			NULL,            // longest class string 
			&cValues,        // number of values for this key 
			&maxValueLen,    // longest value name 
			NULL,            // longest value data 
			NULL,            // security descriptor 
			NULL);            // last write time

	if (result != ERROR_SUCCESS) {
		cout << "\nError RegQueryInfoKey > " << result;
		RegCloseKey(hKey);
		return;
	}
	if (cSubKeys > 0)
	{
		char currentSubkey[MAX_PATH];

		for (int i = 0; i < cSubKeys; i++) {
			DWORD currentSubLen = MAX_PATH;

			retCode = RegEnumKeyExA(hKey,    // Handle to an open/predefined key
				i,                // Index of the subkey to retrieve.
				currentSubkey,            // buffer to receives the name of the subkey
				&currentSubLen,            // size of that buffer
				NULL,                // Reserved
				NULL,                // buffer for class string 
				NULL,                // size of that buffer
				NULL);                // last write time

			if (retCode == ERROR_SUCCESS)
			{
				for (int i = 0; i < tabs; i++)
					printf("\t");
				printf("(%d) %s\n", i + 1, currentSubkey);
				if (flagSub)
				{
					char* subKeyPath = new char[currentSubLen + strlen(subKey)];
					if (strlen(subKey) != 0)
						sprintf(subKeyPath, "%s\\%s", subKey, currentSubkey);
					else
						sprintf(subKeyPath, "%s%s", subKey, currentSubkey);
					PrintSubKeys(RootKey, subKeyPath, flagSub, flagValues, (tabs + 1));
				}
			}
		}
	}
	if (cValues > 0 && flagValues)
	{
		PrintValues(hKey, cValues, (tabs + 1), subKey);
	}
	RegCloseKey(hKey);
}
BOOL SetPrivilege(
    HANDLE hToken,          // access token handle
    LPCTSTR lpszPrivilege,  // name of privilege to enable/disable
    BOOL bEnablePrivilege   // to enable or disable privilege
)
{
    TOKEN_PRIVILEGES tp;
    LUID luid;

    if (!LookupPrivilegeValue(
        NULL,            // lookup privilege on local system
        lpszPrivilege,   // privilege to lookup 
        &luid))        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError());
        return FALSE;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if (bEnablePrivilege)
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.

    if (!AdjustTokenPrivileges(
        hToken,
        FALSE,
        &tp,
        sizeof(TOKEN_PRIVILEGES),
        (PTOKEN_PRIVILEGES)NULL,
        (PDWORD)NULL))
    {
        printf("AdjustTokenPrivileges error: %u\n", GetLastError());
        return FALSE;
    }

    if (GetLastError() == ERROR_NOT_ALL_ASSIGNED)

    {
        printf("The token does not have the specified privilege. \n");
        return FALSE;
    }

    return TRUE;
}
void menu()
{
	stack<string> upwards;
	string cur = "";
	bool printValues = false;
	bool printSubkeys = false;
	while (true)
	{
		
		int c;
		cout << "\nCurrent directory: " << cur << endl;
		if (printSubkeys)
			cout << "SubKeys will be printed out" << endl;
		else
			cout << "SubKeys will not be printed out" << endl;
		if (printValues)
			cout << "Values will be printed out" << endl;
		else
			cout << "Values will not be printed out" << endl;
		cout << "\n1.Print 2.Change subkey flag 3.Change value flag 4.Add value "; 
		cout << "5.Change current directory \n6.Go up 7.Edit value 8.Restore 9.Save 10.Work with git 0.Exit\n";
		cin >> c;
		switch (c)
		{
		case 1:				//Print out the registry folders
			PrintSubKeys(HKEY_CURRENT_USER, cur.c_str(), printSubkeys, printValues);
			break;
		case 2:
			printSubkeys = !printSubkeys;
			break;
		case 3:
			printValues = !printValues;
			break;
		case 4:
		{
			cout << "Choose value type: 1.DWORD 2.Null-terminated string";
			HKEY hkey;
			int choice;
			cin >> choice;
			cout << "Choose value name: ";
			string name;
			cin >> name;
			result = RegOpenKeyExA(HKEY_CURRENT_USER, cur.c_str(), NULL, KEY_ALL_ACCESS, &hkey);
			if (result != ERROR_SUCCESS) {
				cout << "\nError RegOpenKeyEx > " << result;
				RegCloseKey(hkey);
				return;
			}
			switch (choice) 
			{
			case 1:
			{
				DWORD temp;
				cin >> temp;
				result = RegSetValueExA(hkey, name.c_str(),NULL,REG_DWORD,(BYTE*)temp,sizeof(temp));
				if (result != ERROR_SUCCESS) {
					cout << "\nError RegOpenKeyEx > " << result;
					RegCloseKey(hkey);
					return;
				}
				break; 
			}
			case 2:
				string temp;
				cin >> temp;
				result = RegSetValueExA(hkey, name.c_str(), NULL, REG_SZ, (BYTE*)temp.c_str(), sizeof(temp));
				if (result != ERROR_SUCCESS) {
					cout << "\nError RegOpenKeyEx > " << result;
					RegCloseKey(hkey);
					return;
				}
				break;
			}
		
			break; 
		}
		case 5:
		{
			string curr;
			cin >> curr;
			upwards.push(cur);
			if (cur != "")
				cur += "\\";
			cur.append(curr);
			break;
		}
		case 6:
		{
			if (upwards.size() != 0)
			{
				cur = upwards.top();
				upwards.pop();
			}
			break;
		}
		case 7:
		{
			string curr;
			cin >> curr;
			EditValue(HKEY_CURRENT_USER, cur.c_str(), curr.c_str());
			break;
		}
		case 8:
		{
			string curr;
			cin >> curr;
			RestoreRegistry(HKEY_CURRENT_USER, curr);
			break;
		}
		case 9:
		{
			string curr;
			cin >> curr;
			SaveRegistry(HKEY_CURRENT_USER, curr);
			break;
		}
		case 10:
			gitMenu();
			break;
		case 0:
			return;
		default:
			break;
		}
	}
}
void gitMenu()
{
	system("cd registers && git init");
	while (1)
	{
		int c;
		cout << "\n1.Add files 2.Commit 3.Checkout 4.Status 5.Revert 6.User command 0.Exit git menu\n";
		cin >> c;
		switch (c)
		{
		case 1:
		{
			string str;
			cin >> str;
			str = "cd registers && git add " + str;
			system(str.c_str());
			break; 
		}
		case 2:
		{
			string str;
			//cin >> str;
			str = "cd registers && git commit ";
			system(str.c_str());
			break;
		}
		case 3:
		{
			string str;
			cin >> str;
			str = "cd registers && git checkout " + str;
			system(str.c_str());
			break;
		}
		case 4:
			system("cd registers && git status ");
			break;
		case 5:
			system("cd registers && git revert");
			break;
		case 6:
		{
			string str;
			cin >> str;
			str = "cd registers &&" + str;
			system(str.c_str());
			break;
		}
		case 0:
			return;
		default:
			break;
		}
	}
}
int main(int argc,char* argv)
{
	_wmkdir(L"registers");
    //WriteSubKeys(HKEY_CURRENT_USER, "");
    HANDLE ProcessToken;
    HKEY hkey;
    RegOpenKeyA(HKEY_CURRENT_USER, "AlbumArt", &hkey);
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &ProcessToken)) {
		if (SetPrivilege(ProcessToken, SE_BACKUP_NAME, TRUE)==FALSE)
			return -1;
    }
	
	menu();
	return 0;
}