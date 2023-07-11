#pragma once
#include "vec2.h"
#include "font.h"
#include "control.h"

struct Textbox : Control {
	dvec gap;
	double dep = 0;
	int w = 0, h = 0;
	bool clr_upon_edit = false;
	dcol c_txt, c_edit, c_frame, c_normal,
		c_cursor, c_hovered, c_invalid, c_selected;

	// 也是 x_str_rel 的零值，否则一开始编辑文字的位置就会变化。
	int w_edit_margin = 0;
	int w_show_txt_margin = 0;
	double slide_mtp = 0;
	double twinkle_mtp = 0;
	int w_cursor = 0, h_cursor = 0;

	wstring str;
	double x_str_rel = 0;
	int edit_a = 0, edit_b = 0;

	bool edit = false;
	bool enabled = true;
	bool hovered = false;
	bool selecting = false;
	bool cursor_change = false;

	Textbox(int w = 0);
	~Textbox() override;
	int GetW() const override;
	int GetH() const override;

	drect vp_show_txt() const;
	int x_cursor_rel() const;
	int max_x_cursor_rel() const;
	int edit_begin() const;
	int edit_end() const;
	wstring left() const;
	wstring mid() const;
	wstring right() const;
	bool selected() const;

	void select_a();
	void select_b();

	void hdl_left();
	void hdl_right();
	void hdl_delete();
	void hdl_back();
	void hdl_clip();
	void hdl_copy();
	void hdl_paste();
	void hdl_key();
	void hdl_all();

	void add_chs();
	void clamp_cs_selecting();
	void clamp_cs_not_selecting();
	void clamp_cursor();
	void clamp_str();

	void render_main();
	void render_cursor();
	void render();

	virtual void Sync() {}
	virtual void OnDone() const {}
	virtual void Upload() const {}

	void Update() override;
	void Discard() override;
	void PreUpdate() override;
};
