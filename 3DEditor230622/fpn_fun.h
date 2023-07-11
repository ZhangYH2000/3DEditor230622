#pragma once
#include "ctrl_comp.h"
#include "ctrl_list.h"
#include "rich_label.h"
#include "float_panel.h"

struct OtFun : Control {
	double dep = 0;
	int w = 0, h = 0;
	dvec gap, ingap;
	dcol c_edit, c_frame, c_normal, c_hovered, c_fun;
	
	int n_filled = 0;
	vector<double> ys;

	bool edit = false;
	bool draw = false;
	bool hovered = false;

	OtFun();
	~OtFun() override;
	int GetW() const override;
	int GetH() const override;

	void render();
	void Update() override;
	void Discard() override;
	void PreUpdate() override;
};

struct BtSvFun : BtLan {
	BtSvFun();
	void OnClick();
};
struct BtDelFun : BtLan {
	BtDelFun();
	void OnClick();
};
struct RLbFuns : RichLabel {
	RLbFuns();
	void Sync();
};

struct FPnFun : FPnLan {
	ptr<OtFun> ot_fun;
	ptr<Textbox> tb_nm_fun;
	ptr<BtSvFun> bt_sv_fun;
	ptr<BtDelFun> bt_del_fun;
	ptr<CtrlListX> clx_sv_fun;
	ptr<RLbFuns> rlb_funs;
	ptr<CtrlListY> cly;

	FPnFun(App* app);
};
