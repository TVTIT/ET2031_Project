﻿#pragma once
#define VERSION_LABEL L"v0.2.3-beta"
using namespace std;
class Main
{
public:
	static void GetPath();
	static string DirectoryPath;
	static string UnicodeInput();

	static void InitializeConsole();
	static void ClearScreen();

	static vector<string> SplitString(string input, char pattern);
	static bool ValidateDate(string date);

	static void DataInputInvalid();
	static void PauseAndBack();
	static void PauseAndExit();
	
	static void Interface();
private:
};
