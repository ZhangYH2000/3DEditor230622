#include "fpn_language.h"
#include "ui.h"
#include "cur.h"

#define cur ((Cur*)app)
#include "my_def.h"

BtChi::BtChi() : Button(150) { txt = L"ÖÐÎÄ"; }
void BtChi::OnClick() { cur->lan = LAN_CHI; ui.fpn_language->hide(); }

BtEng::BtEng() : Button(150) { txt = L"English"; }
void BtEng::OnClick() { cur->lan = LAN_ENG; ui.fpn_language->hide(); }

FPnLanguage::FPnLanguage(App* app) : FPnLan(app, 200, 200, L"fpn_language") {
	ui.fpns.push_back(this); shown = true;
	vector<Control*> tmp;
	mkp(bt_chi)();
	mkp(bt_eng)();
	tmp = { &*bt_chi, &*bt_eng };
	mkcl(cly); c = &*cly; Init();
}
