#include "stdafx.h"
#include "Dialogs/Dialogs.h"
#include "window_tool/DlgFiles.h"
#include "DiffApp\App.h"
#include "ItemsData\USPCData.h"
namespace 
{
	ItemData<Thickness> &data = Singleton<ItemData<Thickness>>::Instance();

	void Store(FILE *f, wchar_t *name)
	{
		fwprintf(f, L"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		fwprintf(f, L"<PacketXML xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\">\n");
		fwprintf(f, L"<TubeParty>%s</TubeParty>\n", name);
		fwprintf(f, L"<Tubes>\n<Tube>\n");
		fwprintf(f, L"<NumberTube>%s</NumberTube>\n<Zones>\n", name);

		for(int i = 0; i < data.currentOffsetZones; ++i)
		{
			char minV[32] = {};
			sprintf(minV, "%.2f", data.bufferMin[i]);
			for(char *ss = minV; *ss; ++ss){if('.' == *ss){ss += 2;*ss = '\0';break;}}

			char maxV[32] = {};
			sprintf(maxV, "%.2f", data.bufferMax[i]);
			for(char *ss = maxV; *ss; ++ss){if('.' == *ss){ss += 2;*ss = '\0';break;}}

			fwprintf(f, L"<Zone>\n<NumberZone>%d</NumberZone>\n<MinVal>%S</MinVal>\n<MaxVal>%S</MaxVal>\n</Zone>\n"
				, 1 + i
				, minV
				, maxV
				);
		}

		fwprintf(f, L"</Zones>\n</Tube>\n</Tubes>\n");
	}
	
}

void StoreXMLDlg::Do(HWND h)
{
	SaveData o(h, L"Сохранить XML файл");
	wcscpy(o.sFile, app.nameFile);
	o.ofn.lpstrFilter = L"*.xml\0*.xml\0*.*\0*.*\0";
	o.ofn.lpstrDefExt = L"xml";
	if(0 != o.sFile && o())
	{
		FILE *f = _wfopen(o.sFile, L"wt");
		if(f)
		{
			Store(f, app.nameFile);
			fclose(f);
			return;
		}
	}
	MessageBox(h, L"Файл не сохранён", L"Ошибка!!!", MB_ICONERROR);
}