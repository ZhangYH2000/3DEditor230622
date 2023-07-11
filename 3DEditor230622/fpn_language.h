#pragma once
#include "button.h"
#include "ctrl_comp.h"
#include "ctrl_list.h"
#include "float_panel.h"

struct BtChi : Button {
	BtChi();
	void OnClick() override;
};
struct BtEng : Button {
	BtEng();
	void OnClick() override;
};

struct FPnLanguage : FPnLan {
	ptr<BtChi> bt_chi;
	ptr<BtEng> bt_eng;
	ptr<CtrlListY> cly;

	FPnLanguage(App* app);
};
