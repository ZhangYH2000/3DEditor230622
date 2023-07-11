#include "pn_menu.h"
#include "ui.h"
#include "cur.h"
#include "fpn_global.h"
#include "fpn_geo.h"
#include "fpn_par.h"
#include "fpn_fun.h"
#include "fpn_cmd.h"
#include "fpn_language.h"
#include "fpn_about.h"

#define cur ((Cur*)app)
#include "my_def.h"

BtMute::BtMute() : Button(150) {}
void BtMute::Update() {
	txt = cur->mute ? loc(L"unmute") : loc(L"mute");
	Button::Update();
}
void BtMute::OnClick() { cur->mute = !cur->mute; }

// °´Å¥µÄ¿í¶È: 150¡£
PnMenu::PnMenu(App* app) : Panel(Panel::menu(app)) {
	vector<Control*> tmp;
	mkp(bt_mute)();
	mkp(bt_global)(&*ui.fpn_global);
	mkp(bt_geo)(&*ui.fpn_geo);
	mkp(bt_par)(&*ui.fpn_par);
	mkp(bt_fun)(&*ui.fpn_fun);
	mkp(bt_cmd)(&*ui.fpn_cmd);
	mkp(bt_language)(&*ui.fpn_language);
	mkp(bt_about)(&*ui.fpn_about);
	tmp = { &*bt_mute, &*bt_global, &*bt_geo, &*bt_par, &*bt_fun, 
		&*bt_cmd, &*bt_language, &*bt_about };
	mkcl(cly); c = &*cly; Init();
}
