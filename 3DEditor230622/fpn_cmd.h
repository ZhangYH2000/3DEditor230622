#pragma once
#include "ctrl_comp.h"
#include "ctrl_list.h"
#include "float_panel.h"
#include "rich_textbox.h"

struct RTbCmd : RichTextbox {
	RTbCmd();
	void Sync() override;
	void OnDone() const override;
};
struct BtCompile : BtLan {
	BtCompile();
	void OnClick() override;
};

struct FPnCmd : FPnLan {
	ptr<RTbCmd> rtb;
	ptr<BtCompile> bt_compile;
	ptr<CtrlListY> cly;

	FPnCmd(App* app);
};
