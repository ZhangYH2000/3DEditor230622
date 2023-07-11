#include "fpn_about.h"
#include "ui.h"
#include "cur.h"

#define cur ((Cur*)app)
#include "my_def.h"

FPnAbout::FPnAbout(App* app) : FPnLan(app, 800, 600, L"fpn_about") {
	ui.fpns.push_back(this);
	vector<Control*> tmp;
	mkp(rlb)(760, L"about_txt");
	tmp = { &*rlb };
	mkcl(cly); c = &*cly; Init();
}
