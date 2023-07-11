#include "fpn_cmd.h"
#include "ui.h"
#include "cur.h"
#include "var.h"

#define cur ((Cur*)app)
#include "my_def.h"

RTbCmd::RTbCmd() : RichTextbox(560, 500) {}
void RTbCmd::Sync() { str = cur->tmp_cmd; }
void RTbCmd::OnDone() const { cur->tmp_cmd = str; }

BtCompile::BtCompile() : BtLan(200, L"compile") {}
void BtCompile::OnClick() {
	cur->cmd = cur->tmp_cmd;
	auto prog = Compile(cur->cmd);
	Execute(sc, prog);
}

FPnCmd::FPnCmd(App* app) : FPnLan(app, 600, 600, L"fpn_cmd") {
	ui.fpns.push_back(this);
	vector<Control*> tmp;
	mkp(rtb)();
	mkp(bt_compile)();
	tmp = { &*rtb, &*bt_compile };
	mkcl(cly); c = &*cly; Init();
}
