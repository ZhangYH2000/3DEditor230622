#pragma once
#include "ctrl_comp.h"
#include "ctrl_list.h"
#include "separator.h"
#include "float_panel.h"

struct CpParNmDel : TbBt {
	int id = 0;
	CpParNmDel(int id);
	void TbSync() override;
	void TbOnDone() override;
	void BtOnClick() override;
};
struct CpParVal : DbXTb {
	int id = 0;
	CpParVal(int id);
	void DbXSync() override;
	void DbXUpload() override;
	void TbSync() override;
	void TbOnDone() override;
};

struct ClyPar : CtrlListY {
	ptr<CpParNmDel> cp_nm_del;
	ptr<CpParVal> cp_val;
	ptr<SeparatorY> spy;
	ClyPar(int id);
};
struct ClyPars : CtrlListY {
	vector<ptr<ClyPar>> mbs;
	void Update() override;
};

struct BtAddPar : BtLan {
	BtAddPar();
	void OnClick() override;
};

struct FPnPar : FPnLan {
	ptr<ClyPars> cly_pars;
	ptr<BtAddPar> bt_add_par;
	ptr<CtrlListY> cly;

	FPnPar(App* app);
};
