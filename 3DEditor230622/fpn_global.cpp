#include "fpn_global.h"
#include "ui.h"
#include "cur.h"

#define cur ((Cur*)app)
#include "my_def.h"

CpSave::CpSave() : TbBt(270, 80, L"bt_save") {}
void CpSave::BtOnClick() { cur->save(tb->str); }
CpLoad::CpLoad() : TbBt(270, 80, L"bt_load") {}
void CpLoad::BtOnClick() { cur->load(tb->str); }
BtReset::BtReset() : BtLan(200, L"bt_reset") {}
void BtReset::OnClick() { cur->reset(); }
CpPause::CpPause() : LbCb(L"cb_pause") {}
void CpPause::CbSync() { cb->checked = cur->paused; }
void CpPause::CbUpload() { cur->paused = cb->checked; }
CpVol::CpVol() : LbTb(L"tb_vol", 80) {}
void CpVol::TbSync() { tb->str = tw2(cur->vol); }
void CpVol::TbOnDone() { cur->vol = clmp(wtof(tb->str), 0.0, 1.0); }

FPnGlobal::FPnGlobal(App* app) : FPnLan(app, 400, 600, L"fpn_global") {
	ui.fpns.push_back(this);
	vector<Control*> tmp;
	mkp(cp_save)();
	mkp(cp_load)();
	mkp(bt_reset)();
	mkp(cp_pause)();
	mkp(cp_vol)();
	tmp = { &*cp_save, &*cp_load, &*bt_reset, &*cp_pause, &*cp_vol };
	mkcl(cly); c = &*cly; Init();
}
