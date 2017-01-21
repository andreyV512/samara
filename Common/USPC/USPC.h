#pragma once

namespace USPC
{
	bool Config();
	bool Start();
	void Stop();
	bool Open();
	void Close();
	bool Do();
	unsigned &Err();

	void ConfigFromFile();
}