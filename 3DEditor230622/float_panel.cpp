#include "float_panel.h"
#include "app.h"
#include "draw_str.h"
#include "draw_geo.h"
#include "draw_comp.h"
#include "draw_px_seg.h"

#include "ctrl_def.h"

FloatPanel::FloatPanel() {
	dep = 1000;
	txt = L"窗口";
	wheel_mtp = 40;
	tl_show_rel = { 10, 40 };

	h_bar = 30;
	s_close = 20;
	half_s_mark = 5;
	margin_close = 5;
	margin_title = 5;

	c_txt = { 255, 255, 255 };
	c_mark = { 255, 255, 255 };
	c_frame = { 150, 150, 150 };
	c_panel = { 0, 0, 0 };

	c_bar_normal = { 40, 40, 40 };
	c_bar_dragged = { 80, 40, 40 };
	c_bar_hovered = { 40, 40, 80 };

	c_close_normal = { 0, 0, 0 };
	c_close_chosen = { 255, 20, 20 };
	c_close_hovered = { 20, 20, 255 };
	c_close_invalid = { 255, 255, 255 };
}
FloatPanel::FloatPanel(App* _app, int _w, int _h) : FloatPanel() {
	app = _app; vp = scr.rect();
	w = _w; h = _h;
	tl = dvec(scr.w - w, scr.h - h) / 2;
	h_show = h - 50; use_sb = true;

	drect tmp;
	tmp.tl = dvec(10 - 2, 40 - 2);
	tmp.w = w - 40 + 4; tmp.h = h - 50 + 4;
	vp_show_raw_rel = tmp;
}
FloatPanel::~FloatPanel() { rmv; }

int FloatPanel::min_y() const {
	return tl.y + tl_show_rel.y + h_show - c->GetH();
}
drect FloatPanel::vp_show() const {
	drect vp_show_raw = vp_show_raw_rel;
	vp_show_raw.tl += tl;
	return overlap(vp, vp_show_raw);
}
dvec FloatPanel::tl_close() const {
	int x_rel = w - margin_close - s_close;
	int y_rel = (h_bar - s_close) / 2;
	return tl + dvec(x_rel, y_rel);
}
void FloatPanel::set_c(Control* _c) {
	if (c != _c) {
		if (c) { c->Discard(); }
		c = _c;
		if (c) {
			c->app = app;
			c->tl = tl + tl_show_rel;
			c->vp = vp_show();
		}
		sb->c = c;
	}
}
void FloatPanel::set_tl(dvec const& p) {
	dvec dp = p - tl; tl = p;
	if (c) {
		c->tl += dp; c->vp = vp_show();
	}
	sb->tl = tl + dvec(w - sb->w, h_bar);
	sb->top_show = tl.y + tl_show_rel.y;
}
void FloatPanel::init_c() {
	if (c) {
		c->app = app;
		c->tl = tl + tl_show_rel;
		c->vp = vp_show();
	}
}
void FloatPanel::init_sb() {
	mkp(sb)();
	sb->app = app;
	sb->c = c;
	sb->h = h - h_bar;
	sb->tl = tl + dvec(w - sb->w, h_bar);
	sb->vp = vp;
	sb->h_show = h_show;
	sb->top_show = tl.y + tl_show_rel.y;
}

void FloatPanel::hide() {
	shown = false;
	chosen_close = hovered_close =
		hovered = wheeled = hovered_bar = dragged_bar = false;
	sb->Discard();
	if (c) { c->Discard(); }
}
void FloatPanel::show() {
	shown = true; t_click = dur;
}

void FloatPanel::deal_caption() {
	drect rect_close = { tl_close(), s_close, s_close };
	hovered_close = hovered && insd(msp, rect_close);
	hovered_bar = hovered && !hovered_close &&
		insd(msp, { tl, w, h_bar });

	if (dragged_bar) {
		dvec tmp = tl + msp - msp_old;
		tmp.x = clmp(tmp.x, 0, scr.w - w);
		tmp.y = clmp(tmp.y, 0, scr.h - h_bar);
		set_tl(tmp);
		dragged_bar = msd[0];
	}
	else { dragged_bar = msd[0] && hovered_bar && !msd_old[0]; }

	if (chosen_close) {
		bool clicked_close = enabled_close && !msd[0];
		if (clicked_close) { hide(); }
		chosen_close = enabled_close && hovered_close && msd[0];
	}
	else {
		chosen_close = enabled_close && hovered_close &&
			msd[0] && !msd_old[0];
	}
}
void FloatPanel::wheel_and_clamp_cy() {
	if (full) {
		int cy = c->tl.y;
		if (wheeled) { cy += msw * wheel_mtp; }
		cy = clmp(cy, min_y(), tl.y + tl_show_rel.y);
		c->tl.y = cy;
	}
	else if (c) { c->tl.y = tl.y + tl_show_rel.y; }
}

void FloatPanel::render_main() {
	draw_px_rect_framed_raw
	(scr, tl, w, h, vp, c_panel, c_frame);

	dcol const& c_bar =
		dragged_bar ? c_bar_dragged :
		hovered_bar ? c_bar_hovered : c_bar_normal;
	draw_px_rect_framed_raw
	(scr, tl, w, h_bar, vp, c_bar, c_frame);

	dvec tl_title = tl +
		dvec(margin_title, (h_bar - ft.h) / 2);
	draw_str(scr, dscr, dep, txt, c_txt, ft, tl_title, 0, vp);
}
void FloatPanel::render_close() {
	dcol const& c_close =
		!enabled_close ? c_close_invalid :
		chosen_close ? c_close_chosen :
		hovered_close ? c_close_hovered : c_close_normal;
	draw_rect_raw(scr, tl_close(), s_close, s_close, vp, c_close);

	dvec mark_ct = tl_close() + dvec(s_close, s_close) / 2;
	dvec mark_arm = { half_s_mark , half_s_mark };
	draw_px_seg(scr, dscr, mark_ct - mark_arm, mark_ct + mark_arm,
		dep, vp, c_mark);
	mark_arm = { -half_s_mark , half_s_mark };
	draw_px_seg(scr, dscr, mark_ct - mark_arm, mark_ct + mark_arm,
		dep, vp, c_mark);
}
void FloatPanel::render() {
	render_main(); render_close();
}

void FloatPanel::Init() {
	init_c();
	if (use_sb) { init_sb(); }
}
// 这也是比较特殊，相当于要处理自己的 Discard。
void FloatPanel::Update() {
	if (!shown) { return; }
	full = (c != NULL) && h_show < c->GetH();
	hovered = (hvd == this);
	wheeled = (whd == this);

	wheel_and_clamp_cy();
	bool click = hovered && msd[0] && !msd_old[0];
	if (click) { t_click = dur; }
	// 这项摆在最后是因为他可能执行 hide 指令。
	deal_caption();

	// 我觉得这里乱掉了。但是为了这个重构好像不值得。
	if (shown) {
		render();
		if (c) { c->Update(); }
		if (use_sb) { sb->Update(); }
	}
}
void FloatPanel::PreUpdate() {
	if (!shown) return;
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }

	ok = dwh <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dwh = dep; whd = this; }

	if (c) { c->PreUpdate(); }
	if (use_sb) { sb->PreUpdate(); }
}

bool less_fpn(FloatPanel* const fp0, FloatPanel* const fp1) {
	return fp0->t_click < fp1->t_click;
}

FPnLan::FPnLan(App* app, int w, int h, wstring const& id) :
	FloatPanel(app, w, h), id(id) {}
void FPnLan::Update() {
	txt = loc(id); FloatPanel::Update();
}
