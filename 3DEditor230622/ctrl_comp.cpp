#include "ctrl_comp.h"

LbTb::Tb::Tb(LbTb* cp, int w_tb) : cp(cp), Textbox(w_tb) {}
void LbTb::Tb::Sync() { cp->TbSync(); }
void LbTb::Tb::OnDone() const { cp->TbOnDone(); }
LbTb::LbTb(wstring const& id_lb, int w_tb) {
	mkp(lb)(id_lb, true);
	mkp(tb)(this, w_tb);
	cs = { &*lb, &*tb };
}

DlBt::Dl::Dl(DlBt* cp, int w_dl, function<wstring(int)> const& id_dl, int n_dl) :
	cp(cp), DlLan(w_dl, id_dl, n_dl) {}
void DlBt::Dl::Sync() { cp->DlSync(); }
void DlBt::Dl::Upload() const { cp->DlUpload(); }
DlBt::Bt::Bt(DlBt* cp, int w_bt, wstring const& id_bt) :
	cp(cp), BtLan(w_bt, id_bt) {}
void DlBt::Bt::OnClick() { cp->BtOnClick(); }
DlBt::DlBt(int w_dl, function<wstring(int)> const& id_dl,
	int n_dl, int w_bt, wstring const& id_bt) {
	mkp(dl)(this, w_dl, id_dl, n_dl); 
	mkp(bt)(this, w_bt, id_bt);
	cs = { &*dl, &*bt };
}

TbBt::Tb::Tb(TbBt* cp, int w_tb) : cp(cp), Textbox(w_tb) {}
void TbBt::Tb::Sync() { cp->TbSync(); }
void TbBt::Tb::OnDone() const { cp->TbOnDone(); }
TbBt::Bt::Bt(TbBt* cp, int w_bt, wstring const& id_bt) :
	cp(cp), BtLan(w_bt, id_bt) {}
void TbBt::Bt::OnClick() { cp->BtOnClick(); }
TbBt::TbBt(int w_tb, int w_bt, wstring const& id_bt) {
	mkp(tb)(this, w_tb);
	mkp(bt)(this, w_bt, id_bt);
	cs = { &*tb, &*bt };
}

LbDl::Dl::Dl(LbDl* cp, int w_dl,
	function<wstring(int)> const& id_dl, int n_dl) :
	cp(cp), DlLan(w_dl, id_dl, n_dl) {}
void LbDl::Dl::Sync() { cp->DlSync(); }
void LbDl::Dl::Upload() const { cp->DlUpload(); }
LbDl::LbDl(wstring const& id_lb, int w_dl,
	function<wstring(int)> const& id_dl, int n_dl) {
	mkp(lb)(id_lb, true);
	mkp(dl)(this, w_dl, id_dl, n_dl);
	cs = { &*lb, &*dl };
}

LbCb::Cb::Cb(LbCb* cp) : cp(cp) {}
void LbCb::Cb::Sync() { cp->CbSync(); }
void LbCb::Cb::Upload() const { cp->CbUpload(); }
LbCb::LbCb(wstring const& id_lb) {
	mkp(lb)(id_lb);
	mkp(cb)(this);
	cs = { &*lb, &*cb };
};

DbXTb::DbX::DbX(DbXTb* cp, int w_db) : cp(cp), DragbarX(w_db) {}
void DbXTb::DbX::Sync() { cp->DbXSync(); }
void DbXTb::DbX::Upload() const { cp->DbXUpload(); }
DbXTb::Tb::Tb(DbXTb* cp, int w_tb) : cp(cp), Textbox(w_tb) {}
void DbXTb::Tb::Sync() { cp->TbSync(); }
void DbXTb::Tb::OnDone() const { cp->TbOnDone(); }
DbXTb::DbXTb(int w_db, int w_tb) {
	mkp(dbx)(this, w_db);
	mkp(tb)(this, w_tb);
	cs = { &*dbx, &*tb };
}
