#include "RegistryHandler.h"

 LSTATUS RegistryHandler::RestoreRegistry(HKEY RootKey, string filename)
{
	return RegRestoreKeyA(RootKey, filename.c_str(), REG_FORCE_RESTORE);
}

 LSTATUS RegistryHandler::SaveRegistry(HKEY RootKey, string filename)
{
	//RegOpenKeyExA(RootKey,0,);
	remove(filename.c_str());
	return RegSaveKeyExA(RootKey, filename.c_str(), NULL, REG_LATEST_FORMAT);
}

 LSTATUS RegistryHandler::EditValue(HKEY RootKey, const char* subKey, const char* valueName)
{
	HKEY hKey;
	DWORD datatype;
	DWORD dataSize = 1024;
	BYTE data[256];
	result = RegOpenKeyExA(RootKey, subKey, 0, KEY_READ | KEY_WRITE, &hKey);
	if (result != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return result;
	}
	result = RegGetValueA(RootKey, subKey, valueName, REG_DWORD | REG_SZ | REG_LINK, &datatype, &data, &dataSize);
	if (result != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return result;
	}
	if (datatype == REG_DWORD || datatype == REG_NONE) {
		cout << endl << "Please input a number: ";
		int a;
		cin >> a;
		result = RegSetValueExA(hKey, valueName, NULL, datatype, (BYTE*)a, sizeof(a));
	}
	else if (datatype == REG_SZ || datatype == REG_LINK)
	{
		cout << endl << "Please input a string: ";
		string a;
		cin >> a;
		result = RegSetValueExA(hKey, valueName, NULL, datatype, (BYTE*)a.c_str(), a.size());
	}
	RegCloseKey(hKey);
	return result;
}

 LSTATUS RegistryHandler::PrintValues(HKEY hKey, DWORD numValues, int tabs, string subKey)
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
			return result;
		}
		if (datatype == REG_DWORD || datatype == REG_NONE) {
			int a = *data;
			for (int i = 0; i < tabs; i++)
				printf("\t");
			cout << "Value name: " << valueName << endl;
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
			cout << "Value name: " << valueName << endl;
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
	return result;
	//RegCloseKey(hKey);
}

 LSTATUS RegistryHandler::PrintSubKeys(HKEY RootKey, const char* subKey,
	 bool flagSub, bool flagValues, unsigned int tabs)
{
	HKEY hKey;
	DWORD cSubKeys;       
	DWORD maxSubkeyLen;   
	DWORD cValues;     
	DWORD maxValueLen;   
	DWORD retCode;      

	result =
		RegOpenKeyExA(RootKey, subKey, 0, KEY_QUERY_VALUE | 
			KEY_ENUMERATE_SUB_KEYS | KEY_READ , &hKey);
	if (result != ERROR_SUCCESS) {
		RegCloseKey(hKey);
		return result;
	}

	result =
		RegQueryInfoKeyA(hKey,NULL,NULL,NULL,&cSubKeys,
			&maxSubkeyLen,NULL,&cValues,&maxValueLen,NULL,NULL,NULL);						

	if (result != ERROR_SUCCESS) {

		RegCloseKey(hKey);
		return result;
	}
	if (cSubKeys > 0)
	{
		char currentSubkey[MAX_PATH];

		for (int i = 0; i < cSubKeys; i++) {
			DWORD currentSubLen = MAX_PATH;

			retCode = RegEnumKeyExA(hKey,i,currentSubkey,
				&currentSubLen,NULL,NULL,NULL,	NULL);               

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
	return result;
}

 LSTATUS RegistryHandler::addValue(HKEY top, string cur)
{
	cout << "Choose value type: 1.DWORD 2.Null-terminated string";
	HKEY hkey;
	int choice;
	cin >> choice;
	cout << "Choose value name: ";
	string name;
	cin >> name;
	result = RegOpenKeyExA(top, cur.c_str(), NULL, KEY_ALL_ACCESS, &hkey);
	if (result != ERROR_SUCCESS)
	{
		cout << endl << "Error:" << system_category().message(result);
		return result;
	}
	switch (choice)
	{
	case 1:
	{
		DWORD temp;
		cout << "Input a dword:";
		cin >> temp;
		result = RegSetValueExA(hkey, name.c_str(), NULL, REG_DWORD, (BYTE*)temp, sizeof(temp));
		if (result != ERROR_SUCCESS)
		{
			cout << endl << "Error:" << system_category().message(result);
			return result;
		}
		break;
	}
	case 2:
	{	string temp;
		cout << "Input a string:";
		cin >> temp;
		result = RegSetValueExA(hkey, name.c_str(), NULL, REG_SZ, (BYTE*)temp.c_str(), sizeof(temp));
		if (result != ERROR_SUCCESS)
		{
			cout << endl << "Error:" << system_category().message(result);
			return result;
		}
		break; 
	}
	default:
		return ERROR_INVALID_PARAMETER;
	}
	
}

 LSTATUS RegistryHandler::deleteValue(HKEY top, const char* subkey, const char* valuename)
 {
	 return RegDeleteKeyValueA(top, subkey, valuename);
 }

 LSTATUS RegistryHandler::deleteSubkey(HKEY top, const char* subkey)
 {
	 return RegDeleteKeyExA(top, subkey, KEY_WOW64_64KEY | KEY_WOW64_32KEY, 0);
 }
