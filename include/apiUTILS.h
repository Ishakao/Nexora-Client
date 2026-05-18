#pragma once
#include <string>

std::string getLayout();
bool capsLock();
float GetMouseScreenPositionX();
float GetMouseScreenPositionY();
#ifdef _WIN32
	std::wstring GetFile();
#elif __linux__
	std::string GetFile();
#endif