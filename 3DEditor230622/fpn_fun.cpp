#include "fpn_fun.h"
#include "ui.h"
#include "cur.h"
#include "draw_geo.h"
#include "draw_comp.h"

#define cur ((Cur*)app)
#include "my_def.h"
#define nm (ui.fpn_fun->tb_nm_fun->str)

OtFun::OtFun() {
	w = 760; h = 470; dep = 1000;
	ingap = { 10, 10 }; gap = { 10, 10 };

	c_edit = { 80, 40, 40 };
	c_frame = { 150, 150, 150 };
	c_normal = { 40, 40, 40 };
	c_hovered = { 40, 40, 80 };
	c_fun = { 255, 255, 255 };

	int n = w - 2 * ingap.x;
	ys.resize(n);
}
OtFun::~OtFun() { rmv; }
int OtFun::GetW() const { return w + gap.x; }
int OtFun::GetH() const { return h + gap.y; }
void OtFun::render() {
	dcol c = edit ? c_edit :
		hovered ? c_hovered : c_normal;
	int w0 = w - 2 * ingap.x;
	int h0 = h - 2 * ingap.y;
	draw_px_rect_framed_raw(scr, tl, w, h, vp, c, c_frame);
	draw_px_rect_frame(scr, dscr, dep, tl + ingap, w0, h0, vp, c_frame);

	vector<dvec> tmp;
	rep(i, 0, ys.size()) {
		int x = i + tl.x + ingap.x;
		int y = (1 - ys[i]) / 2 * h0 + tl.y + ingap.y;
		tmp.push_back({ x, y });
	}
	draw_px_segstrip(scr, dscr, dep, tmp, vp, c_fun);
}
void OtFun::Update() {
	hovered = (this == hvd);
	if (!edit) {
		bool click = msd[0] && !msd_old[0] && hovered;
		if (click) { edit = true; }
	}
	else {
		if (draw) {
			dvec p = msp - (tl + ingap);
			double y = p.y / (h - 2.0 * ingap.y);
			y = clmp(1 - 2 * y, -1.0, 1.0);

			if (p.x >= 0) {
				if (n_filled == 0) { ys[n_filled] = y; n_filled = 1; }
				int n0 = n_filled - 1;
				int n1 = min<int>(p.x, ys.size() - 1);
				double y0 = ys[n0], y1 = y;
				rep(i, n0 + 1, n1 + 1) {
					ys[i] = y1 * (i - n0) + y0 * (n1 - i); ys[i] /= (n1 - n0);
				}
				n_filled = max(n_filled, n1 + 1);
			}
			draw = msd[0];
		}
		else {
			draw = msd[0] && !msd_old[0] && hovered;
		}

		bool stop = msd[0] && !msd_old[0] && !hovered || n_filled == ys.size();
		if (stop) { edit = draw = false; n_filled = 0; }
	}
	render();
}
void OtFun::Discard() {
	edit = draw = hovered = false;
}
void OtFun::PreUpdate() {
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}

BtSvFun::BtSvFun() : BtLan(80, L"bt_sv_fun") {}
void BtSvFun::OnClick() {
	auto& ys = ui.fpn_fun->ot_fun->ys;
	cur->funs[nm] = msh<vector<double>>(ys);
}
BtDelFun::BtDelFun() : BtLan(80, L"bt_del_fun") {}
void BtDelFun::OnClick() { cur->funs.erase(nm); }
RLbFuns::RLbFuns() : RichLabel(760) {}
void RLbFuns::Sync() {
	txt = loc(L"funs_saved");
	for (auto& f : cur->funs) { txt += f.first + L","; }
}

FPnFun::FPnFun(App* app) : FPnLan(app, 800, 600, L"fpn_fun") {
	ui.fpns.push_back(this);
	vector<Control*> tmp;
	mkp(ot_fun)();
	mkp(tb_nm_fun)(200);
	mkp(bt_sv_fun)();
	mkp(bt_del_fun)();
	tmp = { &*tb_nm_fun, &*bt_sv_fun, &*bt_del_fun };
	mkcl(clx_sv_fun);
	mkp(rlb_funs)();
	tmp = { &*ot_fun, &*clx_sv_fun, &*rlb_funs };
	mkcl(cly); c = &*cly; Init();
}
