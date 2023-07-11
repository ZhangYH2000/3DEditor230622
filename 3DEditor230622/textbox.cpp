#include "textbox.h"
#include "app.h"
#include "draw_geo.h"
#include "draw_str.h"
#include "draw_comp.h"
#include "rich_textbox.h"

#include "ctrl_def.h"

Textbox::Textbox(int w) : w(w) {
	gap = { 10, 10 };
	dep = 1000; h = 30;

	c_txt = { 255, 255, 255 };
	c_edit = { 150, 60, 60 };
	c_frame = { 150, 150, 150 };
	c_normal = { 40, 40, 40 };
	c_cursor = { 255, 255, 255 };
	c_hovered = { 60, 60, 150 };
	c_invalid = { 0, 0, 0 };
	c_selected = { 150, 150, 255 };

	w_edit_margin = 5;
	w_show_txt_margin = 2;
	slide_mtp = 8;
	twinkle_mtp = 8;
	w_cursor = 2, h_cursor = 20;
}
Textbox::~Textbox() { rmv; }
int Textbox::GetW() const { return w + gap.x; }
int Textbox::GetH() const { return h + gap.y; }

drect Textbox::vp_show_txt() const {
	dvec tl_show_txt = tl + dvec(w_show_txt_margin, 0);
	int w_show_txt = w - 2 * w_show_txt_margin;
	return overlap(vp, { tl_show_txt, w_show_txt, h });
}
int Textbox::x_cursor_rel() const {
	wstring tmp(str.begin(), str.begin() + edit_b);
	return x_str_rel + str_wh(tmp, ft, 0).x;
}
// 下面是对的，不需要加一减一什么的。
int Textbox::max_x_cursor_rel() const { return w - w_edit_margin; }
int Textbox::edit_begin() const { return min(edit_a, edit_b); }
int Textbox::edit_end() const { return max(edit_a, edit_b); }
wstring Textbox::left() const {
	auto a = str.begin();
	auto b = str.begin() + edit_begin();
	return wstring(a, b);
}
wstring Textbox::mid() const {
	auto a = str.begin() + edit_begin();
	auto b = str.begin() + edit_end();
	return wstring(a, b);
}
wstring Textbox::right() const {
	auto a = str.begin() + edit_end();
	auto b = str.end();
	return wstring(a, b);
}
bool Textbox::selected() const { return edit_a != edit_b; }

void Textbox::select_a() {
	int x = msp.x - (tl.x + x_str_rel);
	edit_a = edit_b = str_index_at_pos(str, ft, { x, 0 }, 0);
	selecting = true;
}
void Textbox::select_b() {
	int x = msp.x - (tl.x + x_str_rel);
	edit_b = str_index_at_pos(str, ft, { x, 0 }, 0);
	if (!msd[0]) { selecting = false; }
}

void Textbox::hdl_left() {
	if (cursor_change) { selecting = false; }
	if (selected()) { edit_a = edit_b = edit_begin(); }
	else {
		edit_a--;
		if (edit_a < 0) { edit_a = 0; }
		edit_b = edit_a;
	}
}
void Textbox::hdl_right() {
	if (cursor_change) { selecting = false; }
	if (selected()) { edit_a = edit_b = edit_end(); }
	else {
		edit_a++;
		// 下面不要减一。
		if (edit_a > str.size()) { edit_a = str.size(); }
		edit_b = edit_a;
	}
}
void Textbox::hdl_delete() {
	if (cursor_change) { selecting = false; }
	if (selected()) {
		str = left() + right();
		edit_a = edit_b = edit_begin();
	}
	else if (edit_end() < str.size()) {
		auto a = str.begin() + edit_a + 1;
		auto b = str.end();
		str = left() + wstring(a, b);
	}
}
void Textbox::hdl_back() {
	if (cursor_change) { selecting = false; }
	if (selected()) {
		str = left() + right();
		edit_a = edit_b = edit_begin();
	}
	else if (edit_begin() > 0) {
		x_str_rel += ft[str[edit_a]].w;
		if (x_str_rel > w_edit_margin) {
			x_str_rel = w_edit_margin;
		}

		auto a = str.begin();
		auto b = str.begin() + edit_a - 1;
		str = wstring(a, b) + right();
		edit_b = --edit_a;
	}
}
void Textbox::hdl_clip() {
	set_clipboard(mid());
	str = left() + right();
	edit_a = edit_b = edit_begin();
}
void Textbox::hdl_copy() { set_clipboard(mid()); }
void Textbox::hdl_paste() {
	wstring sifted = sift_chs(get_clipboard(), true);
	str = left() + sifted + right();
	edit_a = edit_b = edit_begin() + sifted.size();
}
void Textbox::hdl_all() {
	edit_a = 0; edit_b = str.size();
}
void Textbox::hdl_key() {
	cursor_change = kbd[VK_LEFT] || kbd[VK_RIGHT] ||
		kbd[VK_DELETE] || kbd[VK_BACK];

	if (kbmsg[VK_LEFT]) { hdl_left(); }
	else if (kbmsg[VK_RIGHT]) { hdl_right(); }
	else if (kbmsg[VK_DELETE]) { hdl_delete(); }
	else if (kbmsg[VK_BACK]) { hdl_back(); }

	if (kbd[VK_CONTROL]) {
		if (kbc('C')) { hdl_copy(); }
		else if (kbc('X')) { hdl_clip(); }
		else if (kbc('V')) { hdl_paste(); }
		else if (kbc('A')) { hdl_all(); }
	}
}

void Textbox::add_chs() {
	wstring sifted = sift_chs(chs, true);
	if (!sifted.empty()) {
		str = left() + sifted + right();
		edit_a = edit_b = edit_begin() + sifted.size();
	}
}
void Textbox::clamp_cs_selecting() {
	double val = (msp.x - tl.x) - max_x_cursor_rel();
	if (val > 0) { x_str_rel -= dt * slide_mtp * val; }
	
	val = w_edit_margin - (msp.x - tl.x);
	if (val > 0) { x_str_rel += dt * slide_mtp * val; }
}
void Textbox::clamp_cs_not_selecting() {
	if (x_cursor_rel() > max_x_cursor_rel()) {
		x_str_rel -= x_cursor_rel() - max_x_cursor_rel();
	}
	else if (x_cursor_rel() < w_edit_margin) {
		x_str_rel += w_edit_margin - x_cursor_rel();
	}
}
void Textbox::clamp_cursor() {
	if (selecting) { clamp_cs_selecting(); }
	else { clamp_cs_not_selecting(); }
}
void Textbox::clamp_str() {
	int x_end_rel =
		x_str_rel + str_wh(str, ft, 0).x;
	if (x_end_rel < max_x_cursor_rel()) {
		x_str_rel += max_x_cursor_rel() - x_end_rel;
	}
	if (x_str_rel > w_edit_margin) { x_str_rel = w_edit_margin; }
}

void Textbox::render_main() {
	dcol const& c =
		!enabled ? c_invalid :
		edit ? c_edit :
		hovered ? c_hovered : c_normal;
	draw_px_rect_framed_raw(scr, tl, w, h, vp, c, c_frame);

	int x_cur = 0;
	dvec tl_txt = tl + dvec(x_str_rel, (h - ft.h) / 2);
	draw_str(scr, dscr, dep, left(), c_txt,
		ft, tl_txt, x_cur, 0, vp_show_txt());
	draw_str(scr, dscr, dep, mid(), c_selected,
		ft, tl_txt, x_cur, 0, vp_show_txt());
	draw_str(scr, dscr, dep, right(), c_txt,
		ft, tl_txt, x_cur, 0, vp_show_txt());
}
void Textbox::render_cursor() {
	bool show_cursor = edit &&
		(sin(dur * twinkle_mtp) > 0 || cursor_change);
	if (show_cursor) {
		int x_cursor = tl.x + x_cursor_rel() - w_cursor / 2;
		int y_cursor = tl.y + (h - h_cursor) / 2;

		draw_rect_raw(scr, { x_cursor, y_cursor },
			w_cursor, h_cursor, vp_show_txt(), c_cursor);
	}
}
void Textbox::render() {
	render_main(); render_cursor();
}

void Textbox::Update() {
	hovered = (hvd == this);
	if (edit) {
		bool click = enabled && hovered && msd[0] && !msd_old[0];
		if (click) { select_a(); }
		if (selecting) { select_b(); }
		hdl_key(); add_chs(); clamp_cursor(); Upload();

		bool done = enabled && !hovered && msd[0] && !msd_old[0];
		if (done) { OnDone(); }
		edit = enabled && !(!hovered && msd[0] && !msd_old[0]);
		if (!edit) {
			// 请注意 kb_owner 的登记在 PreUpdate 中进行。
			free_kb(this); selecting = false;
			edit_a = edit_b = 0;
		}
	}
	else {
		Sync();
		edit = enabled && hovered && msd[0] && !msd_old[0];
		if (edit) {
			if (clr_upon_edit) { str.clear(); }
			select_a(); kb = this;
		}
	}
	clamp_str(); render();
}
void Textbox::Discard() {
	free_kb(this);
	edit = hovered = false; selecting = false;
	edit_a = edit_b = 0;
}
void Textbox::PreUpdate() {
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }

	if (edit) { kb = this; }
}
