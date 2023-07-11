#pragma once
#include "ctrl_comp.h"
#include "ctrl_list.h"
#include "rich_label.h"
#include "float_panel.h"

struct FPnAbout : FPnLan {
	ptr<RLbLan> rlb;
	ptr<CtrlListY> cly;

	FPnAbout(App* app);
};
