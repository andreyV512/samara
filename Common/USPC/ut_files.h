#pragma once

void MkDir(wchar_t *);
bool NewUSPCFile(HWND h, wchar_t *);
bool ExistCurrentUSPCFile(wchar_t (&)[256]);
bool GetUsFileParam(double &value, int board, int sensor, wchar_t *name, wchar_t *path);
