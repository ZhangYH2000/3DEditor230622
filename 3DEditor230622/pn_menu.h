#pragma once
#include "panel.h"
#include "button.h"
#include "ctrl_list.h"

struct BtMute : Button {
	BtMute();
	void Update() override;
	void OnClick() override;
};

struct PnMenu : Panel {
	ptr<BtMute> bt_mute;
	ptr<BtFPn> bt_global;
	ptr<BtFPn> bt_geo;
	ptr<BtFPn> bt_par;
	ptr<BtFPn> bt_fun;
	ptr<BtFPn> bt_cmd;
	ptr<BtFPn> bt_language;
	ptr<BtFPn> bt_about;
	ptr<CtrlListY> cly;

	PnMenu(App* app);
};
