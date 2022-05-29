#include "Menu.h"

HKEY Menu::chooseHkey()
{
	while (true) {
		cout << " Please choose what registries you want to view:" << endl;
		cout << " 1.HKEY_CLASSES_ROOT" << endl;
		cout << " 2.HKEY_CURRENT_USER" << endl;
		cout << " 3.HKEY_LOCAL_MACHINE" << endl;
		cout << " 4.HKEY_USERS" << endl;
		cout << " 5.HKEY_CURRENT_CONFIG" << endl;
		int c;
		cin >> c;
		if (!cin.good() || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			c = -1;
		}
		switch (c)
		{
		case 1:
			return HKEY_CLASSES_ROOT;
		case 2:
			return HKEY_CURRENT_USER;
		case 3:
			return HKEY_LOCAL_MACHINE;
		case 4:
			return HKEY_USERS;
		case 5:
			return HKEY_CURRENT_CONFIG;
		default:
			cout << "Wrong input"<<endl;
			break;
		}
	}
}

void Menu::menu(HKEY top)
{
		while (true)
		{

			int c;
			cout << "\n Current directory: " << cur << endl;
			if (printSubkeys)
				cout << " SubKeys will be printed out" << endl;
			else
				cout << " SubKeys will not be printed out" << endl;
			if (printValues)
				cout << " Values will be printed out" << endl;
			else
				cout << " Values will not be printed out" << endl;
			cout << "\n 1.Print 2.Change subkey flag 3.Change value flag 4.Add value "
				<< "5.Change current directory \n 6.Go up 7.Edit value"
				<< " 8.Restore 9.Save 10.Work with git 11.Change current top hive " << endl
				<< " 12.Delete value 13.Delete name 0.Exit "<<endl;
			cin >> c;
			if (!cin.good() || cin.peek()!='\n')
			{
				cin.clear();
				cin.ignore(INT_MAX, '\n');
				c = -1;
			}
			switch (c)
			{
			case 1:
				result = handler.PrintSubKeys(top, cur.c_str(), printSubkeys, printValues, 0);
				if (result != ERROR_SUCCESS)
				{
					cout << endl << "Error:" << result << endl;
				}
				break;
			case 2:
				printSubkeys = !printSubkeys;
				break;
			case 3:
				printValues = !printValues;
				break;
			case 4:
			{
				handler.addValue(top, cur);
				break;
			}
			case 5:
			{
				string curr;
				upwards.push(cur);
				if (cur != "")
					cur += "\\";
				cin >> curr;
				cur.append(curr);
				{
					HKEY key;
					result = RegOpenKeyExA(top, cur.c_str(), 0, KEY_READ | KEY_WRITE, &key);
					if (result != ERROR_SUCCESS) {
						RegCloseKey(key);
						cur = upwards.top();
						upwards.pop();
						cout << "This hive does not exist!" << endl;
						break;
					}
					RegCloseKey(key);
				}

				
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
				cout << "Input valuename to edit:";
				cin >> curr;
				result = handler.EditValue(top, cur.c_str(), curr.c_str());
				if (result != ERROR_SUCCESS)
				{
					cout << endl << "Error:" << system_category().message(result) << endl;
				}
				break;
			}
			case 8:
			{
				string curr;
				cout << "Input filename to restore from:";
				cin >> curr;
				result = handler.RestoreRegistry(top, "registers/" + curr + ".reg");
				if (result != ERROR_SUCCESS)
				{
					cout << endl << "Error:" << system_category().message(result) << endl;
				}
				break;
			}
			case 9:
			{
				string curr;
				cout << "Input filename to backup to:";
				cin >> curr;
				result = handler.SaveRegistry(top, "registers/" + curr + ".reg");
				if (result != ERROR_SUCCESS)
				{
					cout << endl << "Error:" << system_category().message(result) << endl;
				}
				break;
			}
			case 10:
				gitMenu();
				break;
			case 11:
				top = chooseHkey();
				while (upwards.size() != 0)
					upwards.pop();
				cur = "";
				break;
			case 12:
			{
				string name;
				cout << "\nInput value name to delete: ";
				cin >> name;
				result = handler.deleteValue(top, cur.c_str(), name.c_str());
				if (result != ERROR_SUCCESS)
				{
					cout << endl << "Error:" << system_category().message(result) << endl;
				}
				break;
			}
			case 13:
			{
				result = handler.deleteSubkey(top, cur.c_str());
				if (result != ERROR_SUCCESS)
				{
					cout << endl << "Error:" << system_category().message(result) << endl;
				}
				if (upwards.size() != 0)
				{
					cur = upwards.top();
					upwards.pop();
				}
			}
			case 0:
				return;
			default:
				cout << "Wrong input!" << endl;
				break;
			}
		}
}

void Menu::gitMenu()
{
	system("cd registers && git init");
	while (1)
	{
		int c;
		cout << "\n 1.Add files 2.Commit 3.Status 4.Reset 5.Log 6.Remove 7.User command 0.Exit git menu\n";
		cin >> c;
		if (!cin.good() || cin.peek() != '\n')
		{
			cin.clear();
			cin.ignore(INT_MAX, '\n');
			c = -1;
		}
		switch (c)
		{
		case 1:
		{
			string str;
			cout << "Input file name to add";
			cin >> str;
			str = "cd registers && git add " + str;
			system(str.c_str());
			break;
		}
		case 2:
		{
			string str;
			cout << "Input commit message:";
			cin >> str;
			str = "cd registers && git commit -m "+str;
			system(str.c_str());
			break;
		}
		case 3:
		{
			system("cd registers && git status");
			break;
		}
		case 4: 
		{
			string str;
			cout << "Input commit hash:";
			cin >> str;
			str = "cd registers && git reset " + str + " --hard";
			system(str.c_str());
			break; 
		}
		case 5:
			system("cd registers && git log --oneline");
			break;
		case 6:
		{
			string str;
			cout << "Input file name to remove";
			cin >> str;
			str = "cd registers && git rm " + str + " -f";
			system(str.c_str());
			break;
		}
		case 7:
		{
			string str;
			cin >> str;
			str = "cd registers && " + str;
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

BOOL Menu::SetPrivilege(
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