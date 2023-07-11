#include "panel.h"
#include "app.h"
#include "draw_comp.h"

#include "ctrl_def.h"

Panel::Panel() {
	dep = 1000;
	wheel_mtp = 40;
	tl_show_rel = { 10, 10 };

	c_frame = { 150, 150, 150 };
	c_panel = { 0, 0, 0 };
}
Panel::~Panel() { rmv; }
// �˵���ť�Ŀ��: 150.
Panel Panel::menu(App* app) {
	Panel pn;
	pn.app = app; pn.vp = scr.rect();
	pn.w = 169; pn.h = app->h - 40;
	pn.h_show = pn.h - 20;

	drect tmp;
	tmp.tl = dvec(10 - 2, 10 - 2);
	tmp.w = pn.w - 20 + 4; tmp.h = pn.h - 20 + 4;
	pn.vp_show_raw_rel = tmp;
	return pn;
}
Panel Panel::bottom(App* app) {
	Panel pn;
	pn.app = app; pn.vp = scr.rect();
	pn.w = app->w; pn.h = 40;
	pn.tl.y = app->h - pn.h;
	pn.h_show = pn.h;

	drect tmp;
	tmp.tl = dvec(10 - 2, 10 - 2);
	tmp.w = pn.w - 20 + 4; tmp.h = pn.h - 20 + 4;
	pn.vp_show_raw_rel = tmp;
	return pn;
}

int Panel::min_y() const {
	return tl.y + tl_show_rel.y + h_show - c->GetH();
}
drect Panel::vp_show() const {
	drect vp_show_raw = vp_show_raw_rel;
	vp_show_raw.tl += tl;
	return overlap(vp, vp_show_raw);
}
void Panel::set_c(Control* _c) {
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
void Panel::init_c() {
	if (c) {
		c->app = app;
		c->tl = tl + tl_show_rel;
		c->vp = vp_show();
	}
}
void Panel::init_sb() {
	mkp(sb)();
	sb->app = app;
	sb->c = c;
	sb->h = h;
	sb->tl = tl + dvec(w - sb->w, 0);
	sb->vp = vp;
	sb->h_show = h_show;
	sb->top_show = tl.y + tl_show_rel.y;
}
void Panel::render() {
	draw_px_rect_framed_raw(scr, tl, w, h, vp, c_panel, c_frame);
}

void Panel::Init() {
	init_c(); init_sb();
}
void Panel::Update() {
	full = (c != NULL) && h_show < c->GetH();
	hovered = (hvd == this);
	wheeled = (whd == this);

	if (full) {
		int cy = c->tl.y;
		if (wheeled) { cy += msw * wheel_mtp; }
		cy = clmp(cy, min_y(), tl.y + tl_show_rel.y);
		c->tl.y = cy;
	}
	else if (c) { c->tl.y = tl.y + tl_show_rel.y; }

	// ���� Update ˳����Ϊ�˷�����ƣ�����Ӧ�ÿؼ��ȸ��¸߶ȵġ�
	// �����Ϊ���ٶȵ���������֪����û�б�Ҫ��
	// һ�����ܵĸĽ���ʽ���Ƿ��� Update �� Render������δ���á�
	render();
	if (c) { c->Update(); }
	if (use_sb) { sb->Update(); }
}
void Panel::PreUpdate() {
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }

	ok = dwh <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dwh = dep; whd = this; }

	if (c) { c->PreUpdate(); }
	if (use_sb) { sb->PreUpdate(); }
}
