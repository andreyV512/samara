#pragma once
#include <Windows.h>

class RegistryPathLogFile
{
	HKEY hKey;
	wchar_t buffer[1024];
public:
	RegistryPathLogFile();
	~RegistryPathLogFile();
	wchar_t *operator()();
};

class RegistryPathUTFile
{
	HKEY hKey;
	wchar_t buffer[1024];
public:
	RegistryPathUTFile();
	~RegistryPathUTFile();
	wchar_t *operator()();
};

class RegistryPathApplications
{
	HKEY hKey;
	wchar_t buffer[1024];
public:
	RegistryPathApplications();
	~RegistryPathApplications();
	wchar_t *operator()();
};

