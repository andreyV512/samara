#include "stdafx.h"
#include "AutomatAdditional.h"
#include "App/AppBase.h"
#include "templates/templates.hpp"
#include "Stored/PacketBase.h"

namespace AutomatAdditional
{
	void ComputeSpeed(unsigned t)
	{
		wchar_t buf[128];
		buf[0] = 0;
		if(0 != t)
		{
			unsigned referenceOffset = Singleton<AdditionalSettingsTable>::Instance().items.get<ReferenceOffset1>().value;
			double speed = 1000.0 * referenceOffset / t;
			wsprintf(buf, L"Скорость %s мм/сек"
				, Wchar_from<double, 4>(speed)()
				);
		}
		app.MainWindowBottomLabel(App::speed_bottom_label, buf);
	}

	void SetToBottomLabel()
	{
		wchar_t *num = Singleton<NumberPacket>::Instance().value;
		wchar_t *tube = Singleton<NumberTube>::Instance().value;
		wchar_t *op = Singleton<Operator>::Instance().value;
		wchar_t buf[512];
		wsprintf(buf, L"Партия: %s", num);
		app.MainWindowBottomLabel(App::number_party, buf);
		wsprintf(buf, L"Труба: %s", tube);
		app.MainWindowBottomLabel(App::number_tube, buf);
		wsprintf(buf, L"Оператор: %s", op);
		app.MainWindowBottomLabel(App::operator_name, buf);
	}
}