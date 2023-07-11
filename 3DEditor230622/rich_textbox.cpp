#include "rich_textbox.h"
#include "app.h"
#include "draw_geo.h"
#include "draw_str.h"
#include "draw_comp.h"

#include "ctrl_def.h"

RichTextbox::RichTextbox() : RichTextbox(0, 0) {}
RichTextbox::RichTextbox(int w, int h) : w(w), h(h) {
	gap = { 10, 10 };
	dep = 1000;

	c_txt = { 255, 255, 255 };
	c_edit = { 80, 40, 40 };
	c_frame = { 150, 150, 150 };
	c_normal = { 40, 40, 40 };
	c_cursor = { 255, 255, 255 };
	c_hovered = { 40, 40, 80 };
	c_invalid = { 0, 0, 0 };
	c_selected = { 150, 150, 255 };

	edit_margin = { 5, 5 };
	show_txt_margin = { 2, 2 };
	slide_mtp = 8;
	wheel_mtp = 40;
	twinkle_mtp = 8;
	w_cursor = 2, h_cursor = 20;
}
RichTextbox::~RichTextbox() { rmv; }
int RichTextbox::GetW() const { return w + gap.x; }
int RichTextbox::GetH() const { return h + gap.y; }

drect RichTextbox::vp_show_txt() const {
	dvec tl_show_txt = tl + show_txt_margin;
	int w_show_txt = w - 2 * show_txt_margin.x;
	int h_show_txt = h - 2 * show_txt_margin.y;
	return overlap(vp, { tl_show_txt, w_show_txt, h_show_txt });
}
dvec RichTextbox::p_cursor_rel() const {
	wstring tmp(str.begin(), str.begin() + edit_b);
	int x = 0; dvec p = str_wh(tmp, ft, w_txt, &x);
	p.x = x; p.y -= ft.h;
	return (dvec)tl_str_rel + p;
}
int RichTextbox::max_x_cursor_rel() const { return w - edit_margin.x; }
int RichTextbox::max_y_cursor_rel() const { return h - edit_margin.y; }
int RichTextbox::edit_begin() const { return min(edit_a, edit_b); }
int RichTextbox::edit_end() const { return max(edit_a, edit_b); }
wstring RichTextbox::left() const {
	auto a = str.begin();
	auto b = str.begin() + edit_begin();
	return wstring(a, b);
}
wstring RichTextbox::mid() const {
	auto a = str.begin() + edit_begin();
	auto b = str.begin() + edit_end();
	return wstring(a, b);
}
wstring RichTextbox::right() const {
	auto a = str.begin() + edit_end();
	auto b = str.end();
	return wstring(a, b);
}
bool RichTextbox::selected() const { return edit_a != edit_b; }

void RichTextbox::select_a() {
	dvec p = msp - (tl + (dvec)tl_str_rel);
	edit_a = edit_b = str_index_at_pos(str, ft, p, w_txt);
	update_x_cursor_rel_hist();
	selecting = true;
}
void RichTextbox::select_b() {
	dvec p = msp - (tl + (dvec)tl_str_rel);
	edit_b = str_index_at_pos(str, ft, p, w_txt);
	update_x_cursor_rel_hist();
	if (!msd[0]) { selecting = false; }
}
void RichTextbox::update_x_cursor_rel_hist() {
	wstring s = wstring(str.begin(), str.begin() + edit_b);
	int tmp = 0; str_wh(s, ft, w_txt, &tmp);
	x_cursor_rel_hist = tmp;
}

void RichTextbox::hdl_left() {
	if (cursor_change) { selecting = false; }
	if (selected()) { edit_a = edit_b = edit_begin(); }
	else {
		edit_a--;
		if (edit_a < 0) { edit_a = 0; }
		edit_b = edit_a;
	}
	update_x_cursor_rel_hist();
}
void RichTextbox::hdl_right() {
	if (cursor_change) { selecting = false; }
	if (selected()) { edit_a = edit_b = edit_end(); }
	else {
		edit_a++;
		// ���治Ҫ��һ��
		if (edit_a > str.size()) { edit_a = str.size(); }
		edit_b = edit_a;
	}
	update_x_cursor_rel_hist();
}
void RichTextbox::hdl_up() {
	if (cursor_change) { selecting = false; }
	if (selected()) { edit_a = edit_b = edit_begin(); }
	else {
		wstring tmp(str.begin(), str.begin() + edit_b);
		int y = str_wh(tmp, ft, w_txt).y - 2 * ft.h;
		dvec p(x_cursor_rel_hist, y);
		edit_a = edit_b = str_index_at_pos(str, ft, p, w_txt);
	}
}
void RichTextbox::hdl_down() {
	if (cursor_change) { selecting = false; }
	if (selected()) { edit_a = edit_b = edit_end(); }
	else {
		wstring tmp(str.begin(), str.begin() + edit_b);
		int y = str_wh(tmp, ft, w_txt).y;
		dvec p(x_cursor_rel_hist, y);
		edit_a = edit_b = str_index_at_pos(str, ft, p, w_txt);
	}
}
void RichTextbox::hdl_delete() {
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
	// ������ delete ���ø��µġ�
	// update_x_cursor_rel_hist();
}
void RichTextbox::hdl_back() {
	if (cursor_change) { selecting = false; }
	if (selected()) {
		str = left() + right();
		edit_a = edit_b = edit_begin();
	}
	else if (edit_begin() > 0) {
		auto a = str.begin();
		auto b = str.begin() + edit_a - 1;
		str = wstring(a, b) + right();
		edit_b = --edit_a;
	}
	update_x_cursor_rel_hist();
}
void RichTextbox::hdl_clip() {
	set_clipboard(mid());
	str = left() + right();
	edit_a = edit_b = edit_begin();
}
void RichTextbox::hdl_copy() { set_clipboard(mid()); }
void RichTextbox::hdl_paste() {
	wstring sifted = sift_chs(get_clipboard(), false);
	str = left() + sifted + right();
	edit_a = edit_b = edit_begin() + sifted.size();
}
void RichTextbox::hdl_all() {
	edit_a = 0; edit_b = str.size();
}
void RichTextbox::hdl_key() {
	cursor_change =
		kbd[VK_LEFT] || kbd[VK_RIGHT] ||
		kbd[VK_DELETE] || kbd[VK_BACK];

	if (kbmsg[VK_LEFT]) { hdl_left(); }
	else if (kbmsg[VK_RIGHT]) { hdl_right(); }
	else if (kbmsg[VK_UP]) { hdl_up(); }
	else if (kbmsg[VK_DOWN]) { hdl_down(); }
	else if (kbmsg[VK_DELETE]) { hdl_delete(); }
	else if (kbmsg[VK_BACK]) { hdl_back(); }

	if (kbd[VK_CONTROL]) {
		if (kbc('C')) { hdl_copy(); }
		else if (kbc('X')) { hdl_clip(); }
		else if (kbc('V')) { hdl_paste(); }
		else if (kbc('A')) { hdl_all(); }
	}
}

void RichTextbox::add_chs() {
	wstring sifted = sift_chs(chs, false);
	if (!sifted.empty()) {
		str = left() + sifted + right();
		edit_a = edit_b = edit_begin() + sifted.size();
		update_x_cursor_rel_hist();
	}
}
void RichTextbox::clamp_cs_selecting() {
	double val = (msp.x - tl.x) - max_x_cursor_rel();
	if (val > 0) { tl_str_rel.x -= dt * slide_mtp * val; }
	
	val = edit_margin.x - (msp.x - tl.x);
	if (val > 0) { tl_str_rel.x += dt * slide_mtp * val; }
	
	val = (msp.y - tl.y) - max_y_cursor_rel();
	if (val > 0) { tl_str_rel.y -= dt * slide_mtp * val; }

	val = edit_margin.y - (msp.y - tl.y);
	if (val > 0) { tl_str_rel.y += dt * slide_mtp * val; }
}
void RichTextbox::clamp_cs_not_selecting() {
	if (p_cursor_rel().x > max_x_cursor_rel()) {
		tl_str_rel.x -= p_cursor_rel().x - max_x_cursor_rel();
	}
	else if (p_cursor_rel().x < edit_margin.x) {
		tl_str_rel.x += edit_margin.x - p_cursor_rel().x;
	}
	// ���ǲ��� y ��ô����
}
void RichTextbox::clamp_cursor() {
	if (selecting) { clamp_cs_selecting(); }
	else { clamp_cs_not_selecting(); }
}
void RichTextbox::clamp_str() {
	// �������ֹ������⣬clamp_selecting �� wheel ��Ҳ��Ҫ��һ����
	dvec p_end_rel =
		(dvec)tl_str_rel + str_wh(str, ft, w_txt);
	if (p_end_rel.x < max_x_cursor_rel()) {
		tl_str_rel.x += max_x_cursor_rel() - p_end_rel.x;
	}
	if (p_end_rel.y < max_y_cursor_rel()) {
		tl_str_rel.y += max_y_cursor_rel() - p_end_rel.y;
	}
	// �����������������ġ�
	if (tl_str_rel.x > edit_margin.x) { tl_str_rel.x = edit_margin.x; }
	if (tl_str_rel.y > edit_margin.y) { tl_str_rel.y = edit_margin.y; }
}

void RichTextbox::render_main() {
	dcol c =
		!enabled ? c_invalid :
		edit ? c_edit :
		hovered ? c_hovered : c_normal;
	draw_px_rect_framed_raw(scr, tl, w, h, vp, c, c_frame);

	int x_cur = 0;
	dvec tl_txt = tl + (dvec)tl_str_rel;
	draw_str(scr, dscr, dep, left(), c_txt,
		ft, tl_txt, x_cur, w_txt, vp_show_txt());
	draw_str(scr, dscr, dep, mid(), c_selected,
		ft, tl_txt, x_cur, w_txt, vp_show_txt());
	draw_str(scr, dscr, dep, right(), c_txt,
		ft, tl_txt, x_cur, w_txt, vp_show_txt());
}
void RichTextbox::render_cursor() {
	bool show_cursor = edit &&
		(sin(dur * twinkle_mtp) > 0 || cursor_change);
	if (show_cursor) {
		dvec tl_cursor = tl + p_cursor_rel() + 
			dvec(-w_cursor, ft.h - h_cursor) / 2;
		draw_rect_raw(scr, tl_cursor,
			w_cursor, h_cursor, vp_show_txt(), c_cursor);
	}
}
void RichTextbox::render() {
	render_main(); render_cursor();
}

void RichTextbox::Update() {
	hovered = (hvd == this);
	bool wheeled = (whd == this);
	if (edit) {
		bool click = enabled && hovered && msd[0] && !msd_old[0];
		if (click) { select_a(); }
		if (selecting) { select_b(); }
		hdl_key(); add_chs(); clamp_cursor(); Upload();

		bool done = enabled && !hovered && msd[0] && !msd_old[0];
		if (done) { OnDone(); }
		edit = enabled && !(!hovered && msd[0] && !msd_old[0]);
		if (!edit) {
			// ��ע�� kb_owner �ĵǼ��� PreUpdate �н��С�
			free_kb(this); selecting = false;
			edit_a = edit_b = 0; x_cursor_rel_hist = 0;
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
	if (wheeled) { tl_str_rel.y += msw * wheel_mtp; }
	clamp_str(); render();
}
void RichTextbox::Discard() {
	free_kb(this);
	edit = hovered = false; selecting = false;
	edit_a = edit_b = 0; x_cursor_rel_hist = 0;
}
void RichTextbox::PreUpdate() {
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }

	ok = dwh <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dwh = dep; whd = this; }

	if (edit) { kb = this; }
}
