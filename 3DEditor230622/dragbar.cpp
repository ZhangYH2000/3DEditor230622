#include "dragbar.h"
#include "app.h"
#include "draw_geo.h"

#include "ctrl_def.h"

DragbarX::DragbarX(int w) : bw(w) {
	gap = { 10, 10 };
	dep = 1000;
	bh = 2; nw = 12, nh = 30;

	c_bar = { 80, 80, 80 };
	c_normal = { 80, 80, 80 };
	c_hovered = { 20, 20, 255 };
	c_invalid = { 0, 0, 0 };
	c_dragged = { 250, 20, 20 };
}
DragbarX::~DragbarX() { rmv; }
int DragbarX::GetW() const { return bw + gap.x; }
int DragbarX::GetH() const { return nh + gap.y; }

// 下面的表达式是正确的，不需要加一减一什么的。
double DragbarX::uval() const {
	return (double)nx_rel / (bw - nw);
}
void DragbarX::set_nx_rel(double uv) {
	int tmp = uv * (bw - nw);
	nx_rel = clmp(tmp, 0, bw - nw);
}
dvec DragbarX::tl_node() const { 
	return tl + dvec(nx_rel, 0);
}
void DragbarX::render() {
	dcol const& c =
		!enabled ? c_invalid :
		dragged ? c_dragged :
		hovered ? c_hovered : c_normal;
	dvec tl_bar = tl + dvec(0, nh - bh) / 2;
	draw_rect_raw(scr, tl_bar, bw, bh, vp, c_bar);
	draw_rect_raw(scr, tl_node(), nw, nh, vp, c);
}

void DragbarX::Update() {
	hovered = (hvd == this);
	if (dragged) {
		int x_zero = tl.x + nw / 2;
		nx_rel = clmp(msp.x - x_zero, 0, bw - nw);
		Upload();

		bool done = enabled && !msd[0];
		if (done) { OnDone(); }
		dragged = enabled && msd[0];
	}
	else {
		Sync();
		dragged = enabled && msd[0] && hovered && !msd_old[0];
	}
	render();
}
void DragbarX::Discard() { dragged = hovered = false; }
void DragbarX::PreUpdate() {
	bool ok = dhv <= dep &&
		insd(msp, { tl_node(), nw, nh }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}
