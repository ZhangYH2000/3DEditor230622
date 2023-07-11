#include "button.h"
#include "app.h"
#include "draw_str.h"
#include "draw_comp.h"
#include "float_panel.h"

#include "ctrl_def.h"

Button::Button(int w) : w(w) {
	gap = { 10, 10 };
	txt = L"°´Å¥";
	dep = 1000; h = 30;

	c_txt = { 255, 255, 255 };
	c_frame = { 150, 150, 150 };
	c_normal = { 80, 80, 80 };
	c_chosen = { 150, 60, 60 };
	c_hovered = { 60, 60, 150 };
	c_invalid = { 0, 0, 0 };
}
Button::~Button() { rmv; }
int Button::GetW() const { return w + gap.x; }
int Button::GetH() const { return h + gap.y; }

void Button::render() {
	dcol const& c =
		!enabled ? c_invalid :
		chosen ? c_chosen :
		hovered ? c_hovered : c_normal;
	dvec _wh = str_wh(txt, ft, 0);
	dvec tl_txt = tl + dvec(w, h) / 2 - _wh / 2;

	draw_px_rect_framed_raw(scr, tl, w, h, vp, c, c_frame);
	draw_str(scr, dscr, dep, txt, c_txt, ft, tl_txt, 0, vp);
}

void Button::Update() {
	hovered = (hvd == this);
	if (chosen) {
		bool clicked = enabled && !msd[0];
		if (clicked) { OnClick(); }
		chosen = enabled && hovered && msd[0];
	}
	else {
		chosen = enabled && hovered && msd[0] && !msd_old[0];
	}
	render();
}
void Button::Discard() { chosen = hovered = false; }
void Button::PreUpdate() {
	bool ok = dhv <= dep &&
		insd(msp, { tl, w, h }) && insd(msp, vp);
	if (ok) { dhv = dep; hvd = this; }
}

BtLan::BtLan(int w, wstring const& id) : Button(w), id(id) {}
void BtLan::Update() {
	txt = loc(id); Button::Update();
}

BtFPn::BtFPn(FloatPanel* fpn) : Button(150), fpn(fpn) {}
void BtFPn::OnClick() { fpn->show(); }
void BtFPn::Update() {
	enabled = !fpn->shown;
	txt = fpn->txt;
	Button::Update();
}
