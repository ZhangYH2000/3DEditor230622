#include "fpn_par.h"
#include "ui.h"
#include "cur.h"
#include "par.h"

#define cur ((Cur*)app)
#include "my_def.h"
#define par (*cur->pars[id])

CpParNmDel::CpParNmDel(int id) : id(id), TbBt(270, 80, L"bt_del_par") {}
void CpParNmDel::TbSync() { tb->str = par.nm; }
void CpParNmDel::TbOnDone() { par.nm = tb->str; }
void CpParNmDel::BtOnClick() { par.del = true; }

CpParVal::CpParVal(int id) : id(id), DbXTb(270, 80) {}
void CpParVal::DbXSync() { dbx->set_nx_rel(par.val); }
void CpParVal::DbXUpload() { par.val = dbx->uval(); }
void CpParVal::TbSync() { tb->str = tw2(par.val); }
void CpParVal::TbOnDone() {
	par.val = clmp(wtof(tb->str), 0.0, 1.0);
}

ClyPar::ClyPar(int id) {
	mkp(cp_nm_del)(id);
	mkp(cp_val)(id);
	mkp(spy)(400);
	cs = { &*cp_nm_del, &*cp_val, &*spy };
}
void ClyPars::Update() {
	rep(i, mbs.size(), cur->pars.size()) {
		mbs.push_back(msh<ClyPar>(i));
	}
	cs.clear();
	rep(i, 0, cur->pars.size()) {
		cs.push_back(&*mbs[i]);
	}
	rep(i, cur->pars.size(), mbs.size()) {
		mbs[i]->Discard();
	}
	CtrlListY::Update();
}
BtAddPar::BtAddPar() : BtLan(160, L"bt_add_par") {}
void BtAddPar::OnClick() { cur->pars.push_back(msh<param>(loc(L"unnamed"))); }

FPnPar::FPnPar(App* app) : FPnLan(app, 400, 600, L"fpn_par") {
	ui.fpns.push_back(this);
	vector<Control*> tmp;
	mkp(cly_pars)();
	mkp(bt_add_par)();
	tmp = { &*cly_pars, &*bt_add_par };
	mkcl(cly); c = &*cly; Init();
}
