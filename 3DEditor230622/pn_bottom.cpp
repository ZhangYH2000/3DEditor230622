#include "pn_bottom.h"
#include "cur.h"
#include "bgr.h"

#define cur ((Cur*)app)
#include "my_def.h"

LbFPS::LbFPS() {
	w = 450; fixed_w = true;
}
void LbFPS::Sync() {
	txt =
		loc(L"frm_tm") + tw2(cur->fps.frm_time * 1000) +
		L", FPS: " + tw2(cur->fps.fps);
}
LbCam::LbCam() {
	w = 600; fixed_w = true;
}
void LbCam::Sync() {
	txt =
		loc(L"cam_stat") + L"theta=" + tw2(cm.theta) + L",phi=" + tw2(cm.phi) +
		L",psi=" + tw2(cm.psi) + L",scl=" + tw((int)cm.scl) + 
		L",r=" + tw((int)cm.r) + L",look=" + tw2(cm.look);
}

PnBottom::PnBottom(App* app) : Panel(Panel::bottom(app)) {
	vector<Control*> tmp;
	mkp(lb_fps)();
	mkp(lb_cam)();
	tmp = { &*lb_fps, &*lb_cam };
	mkcl(clx); c = &*clx; Init();
}
