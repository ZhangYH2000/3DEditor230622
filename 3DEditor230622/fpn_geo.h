#pragma once
#include "ctrl_comp.h"
#include "ctrl_list.h"
#include "separator.h"
#include "float_panel.h"
#include "rich_textbox.h"

struct RTbGeo : RichTextbox {
	int id;
	RTbGeo(int id);
	void Sync() override;
	void OnDone() const override;
};
struct BtDelGeo : BtLan {
	int id;
	BtDelGeo(int id);
	void OnClick() override;
};
struct BtUpdateGeo : BtLan {
	int id;
	BtUpdateGeo(int id);
	void OnClick() override;
};
struct CbShowGeo : Checkbox {
	int id;
	CbShowGeo(int id);
	void Sync() override;
	void Upload() const override;
};
struct CbGeoSelected : Checkbox {
	int id;
	CbGeoSelected(int id);
	void Sync() override;
	void Upload() const override;
};

struct ClyGeo : CtrlListY {
	ptr<RTbGeo> rtb;
	ptr<BtDelGeo> bt_del;
	ptr<BtUpdateGeo> bt_update;
	ptr<LbLan> lb_show;
	ptr<CbShowGeo> cb_show;
	ptr<LbLan> lb_selected;
	ptr<CbGeoSelected> cb_selected;
	ptr<CtrlListX> clx;
	ptr<SeparatorY>  spy;

	ClyGeo(int id);
};
struct ClyGeos : CtrlListY {
	vector<ptr<ClyGeo>> mbs;
	void Update() override;
};
struct BtCrtGeo : BtLan {
	BtCrtGeo();
	void OnClick() override;
};

struct FPnGeo : FPnLan {
	ptr<ClyGeos> cly_geos;
	ptr<BtCrtGeo> bt_crt_geo;
	ptr<CtrlListY> cly;

	FPnGeo(App* app);
};
