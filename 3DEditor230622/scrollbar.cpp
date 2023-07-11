#include "scrollbar.h"
#include "app.h"
#include "draw_comp.h"

#include "ctrl_def.h"

ScrollbarY::ScrollbarY() {
	dep = 1000; w = 20;

	c_bar = { 0, 0, 0 };
	c_frame = { 150, 150, 150 };
	c_normal = { 80, 80, 80 };
	c_dragged = { 150, 60, 60 };
	c_hovered = { 60, 60, 150 };
	c_invalid = { 0, 0, 0 };
}
ScrollbarY::~ScrollbarY() { rmv; }
void ScrollbarY::render() {
	draw_px_rect_framed_raw
	(scr, tl, w, h, vp, c_bar, c_frame);

	if (full) {
		dcol const& c =
			!enabled ? c_invalid :
			dragged ? c_dragged :
			hovered_node ? c_hovered : c_normal;
		draw_px_rect_framed_raw
		(scr, tl + dvec(0, ny_rel), w, nh, vp, c, c_frame);
	}
}

void ScrollbarY::Update() {
	full = (c != NULL) && h_show < c->GetH();
	hovered = (hvd == this);
	hovered_node = hovered &&
		insd(msp, { tl + dvec(0, ny_rel), w, nh });

	// 这个状态变化比一般的控件是要复杂的。
	if (full) {
		nh = h_show * h / c->GetH();

		if (dragged) {
			ny_rel += msp.y - msp_old.y;
			ny_rel = clmp(ny_rel, 0, h - nh);
			int cy_rel = ny_rel * c->GetH() / h;
			c->tl.y = top_show - cy_rel;

			dragged = full && enabled && msd[0];
		}
		else {
			int cy_rel = top_show - c->tl.y;
			ny_rel = cy_rel * h / c->GetH();
			ny_rel = clmp(ny_rel, 0, h - nh);

			dragged = full && enabled && msd[0] &&
				hovered_node && !msd_old[0];
		}
	}
	else { dragged = false; }
	render();
}
void ScrollbarY::Discard() {
	dragged = hovered = hovered_node = full = false;
}
void ScrollbarY::PreUpdate() {
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}
