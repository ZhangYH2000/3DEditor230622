#include "droplist.h"
#include "app.h"
#include "draw_geo.h"
#include "draw_str.h"
#include "draw_comp.h"

#include "ctrl_def.h"

wstring nm_none(int id) { return L"Пе"; }

Droplist::Droplist(int w) : w(w) {
	gap = { 10, 10 };
	dep = 1000;
	h_item = 25; h = 30;

	sign_r = 5;
	sign_margin = 15;
	x_text_margin = 5;

	c_txt = { 255, 255, 255 };
	c_mark = { 255, 255, 255 };
	c_edit = { 150, 60, 60 };
	c_frame = { 150, 150, 150 };
	c_normal = { 40, 40, 40 };
	c_invalid = { 0, 0, 0 };
	c_hovered = { 60, 60, 150 };
}
Droplist::~Droplist() { rmv; }
int Droplist::GetW() const { return w + gap.x; }
int Droplist::GetH() const {
	return h + gap.y + (edit ? n_item * h_item : 0);
}

dvec Droplist::tl_item() const { return tl + dvec(0, h); }
void Droplist::render_main() {
	dcol const& c =
		!enabled ? c_invalid :
		edit ? c_edit :
		hovered ? c_hovered : c_normal;
	draw_px_rect_framed(scr, dscr, dep, tl, w, h, vp, c, c_frame);

	vec2 ct_sign = (vec2)tl +
		vec2(w - sign_margin, h / 2);
	draw_eclipse(scr, dscr, dep, ct_sign, sign_r, sign_r, vp, c_mark);

	dvec tl_txt = tl +
		dvec(x_text_margin, (h - ft.h) / 2);
	draw_str(scr, dscr, dep, nm(picked), c_txt, ft, tl_txt, 0, vp);
}
void Droplist::render_items() {
	if (edit) {
		draw_px_rect_framed(scr, dscr, dep, tl_item(), 
			w, h_item * n_item, vp, c_normal, c_frame);

		if (item_hv != -1) {
			dvec tl_hovered = tl + 
				dvec(0, h + h_item * item_hv);
			draw_px_rect_framed
			(scr, dscr, dep, tl_hovered, w, h_item, vp, c_hovered, c_frame);
		}

		rep(i, 0, n_item) {
			dvec tl_txt_item = tl + 
				dvec(0, h + h_item * i) +
				dvec(x_text_margin, (h_item - ft.h) / 2);
			draw_str(scr, dscr, dep, nm(i), c_txt, ft, tl_txt_item, 0, vp);
		}
	}
}
void Droplist::render() {
	render_main(); render_items();
}

void Droplist::Update() {
	hovered = (hvd == this);
	if (edit) {
		if (hovered) {
			int tmp = msp.y - tl.y - h;
			if (tmp >= 0) { item_hv = tmp / h_item; }
			else { item_hv = -1; }
		}
		bool done = enabled && hovered && msd[0] && !msd_old[0];
		if (done) {
			if (item_hv != -1) { picked = item_hv; }
			Upload();
		}
		edit = enabled && !(msd[0] && !msd_old[0]);
		if (!edit) { item_hv = -1; }
	}
	else {
		Sync();
		edit = enabled && hovered && msd[0] && !msd_old[0];
	}
	render();
}
void Droplist::Discard() {
	edit = hovered = false;
	item_hv = -1;
}
void Droplist::PreUpdate() {
	int real_h = h +
		(edit ? n_item * h_item : 0);
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, real_h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}

DlLan::DlLan(int w, function<wstring(int)> const& _id, int n) :
	Droplist(w), id(_id) {
	n_item = n;
	nm = [this](int i) { return loc(id(i)); };
}
