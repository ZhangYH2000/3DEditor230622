#pragma once
#include "ctrl_comp.h"
#include "ctrl_list.h"
#include "float_panel.h"

struct CpSave : TbBt {
	CpSave();
	void BtOnClick() override;
};
struct CpLoad : TbBt {
	CpLoad();
	void BtOnClick() override;
};
struct BtReset : BtLan {
	BtReset();
	void OnClick() override;
};
struct CpPause : LbCb {
	CpPause();
	void CbSync() override;
	void CbUpload() override;
};
struct CpVol : LbTb {
	CpVol();
	void TbSync() override;
	void TbOnDone() override;
};

struct FPnGlobal : FPnLan {
	ptr<CpSave> cp_save;
	ptr<CpLoad> cp_load;
	ptr<BtReset> bt_reset;
	ptr<CpPause> cp_pause;
	ptr<CpVol> cp_vol;
	ptr<CtrlListY> cly;

	FPnGlobal(App* app);
};
