#include "fpn_geo.h"
#include "ui.h"
#include "cur.h"
#include "geo.h"

#define cur ((Cur*)app)
#include "my_def.h"
#define geos (cur->geos)
#define geo (*geos[id])

RTbGeo::RTbGeo(int id) : id(id), RichTextbox(360, 200) {}
void RTbGeo::Sync() { str = geo.tmp_cmd; }
void RTbGeo::OnDone() const { geo.tmp_cmd = str; }
BtUpdateGeo::BtUpdateGeo(int id) : id(id), BtLan(70, L"bt_update_geo") {}
void BtUpdateGeo::OnClick() { geo.cmd = geo.tmp_cmd; }
BtDelGeo::BtDelGeo(int id) : id(id), BtLan(70, L"bt_del_geo") {}
void BtDelGeo::OnClick() { geo.del = true; }
CbShowGeo::CbShowGeo(int id) : id(id), Checkbox(true) {}
void CbShowGeo::Sync() { checked = geo.show; }
void CbShowGeo::Upload() const { geo.show = checked; }
CbGeoSelected::CbGeoSelected(int id) : id(id), Checkbox(true) {}
void CbGeoSelected::Sync() { checked = &geo == gsel; }
void CbGeoSelected::Upload() const { gsel = checked ? &geo : NULL; }

ClyGeo::ClyGeo(int id) {
	vector<Control*> tmp;
	mkp(rtb)(id);
	mkp(bt_update)(id);
	mkp(bt_del)(id);
	mkp(lb_show)(L"cb_show_geo", true);
	mkp(cb_show)(id);
	mkp(lb_selected)(L"cb_geo_selected", true);
	mkp(cb_selected)(id);
	tmp = { &*bt_update, &*bt_del, &*lb_show, &*cb_show, &*lb_selected, &*cb_selected };
	mkcl(clx);
	mkp(spy)(400);
	cs = { &*rtb, &*clx, &*spy };
}

void ClyGeos::Update() {
	rep(i, mbs.size(), geos.size()) {
		mbs.push_back(msh<ClyGeo>(i));
	}
	cs.clear();
	rep(i, 0, geos.size()) {
		cs.push_back(&*mbs[i]);
	}
	rep(i, geos.size(), mbs.size()) {
		mbs[i]->Discard();
	}
	CtrlListY::Update();
}

BtCrtGeo::BtCrtGeo() : BtLan(200, L"bt_crt_geo") {}
void BtCrtGeo::OnClick() {
	geos.push_back(msh<Geo>(cur));
}

FPnGeo::FPnGeo(App* app) : FPnLan(app, 400, 600, L"fpn_geo") {
	ui.fpns.push_back(this);
	vector<Control*> tmp;
	mkp(cly_geos)();
	mkp(bt_crt_geo)();
	tmp = { &*cly_geos, &*bt_crt_geo };
	mkcl(cly); c = &*cly; Init();
}
