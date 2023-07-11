#include "label.h"
#include "app.h"
#include "draw_str.h"

#include "ctrl_def.h"

Label::Label(bool big) {
	h = big ? 30 : 20;
	gap = { 10, 10 };
	dep = 1000;
	txt = L"ÎÄ±¾";
	c_txt = { 255, 255, 255 };
}
int Label::GetW() const { return w + gap.x; }
int Label::GetH() const { return h + gap.y; }

void Label::render() {
	dvec tl_txt = tl + dvec(0, h - ft.h) / 2;
	draw_str(scr, dscr, dep, txt, c_txt, ft, tl_txt, 0, vp);
}
void Label::Update() {
	Sync();
	if (!fixed_w) { w = str_wh(txt, ft, 0).x; }
	render();
}

LbLan::LbLan(wstring const& id, bool big) : Label(big), id(id) {}
void LbLan::Sync() { txt = loc(id); }
