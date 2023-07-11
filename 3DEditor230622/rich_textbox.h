#pragma once
#include "vec2.h"
#include "font.h"
#include "control.h"

struct RichTextbox : Control {
	dvec gap;
	int w_txt = 0;
	double dep = 0;
	int w = 0, h = 0;
	bool clr_upon_edit = false;
	dcol c_txt, c_edit, c_frame, c_normal,
		c_cursor, c_hovered, c_invalid, c_selected;

	dvec edit_margin;
	dvec show_txt_margin;
	double slide_mtp = 0;
	double wheel_mtp = 0;
	double twinkle_mtp = 0;
	int w_cursor = 0, h_cursor = 0;

	wstring str;
	vec2 tl_str_rel;
	int edit_a = 0, edit_b = 0;
	// 下面的 rel 是相对于 tl_str_rel 的。
	int x_cursor_rel_hist = 0;

	bool edit = false;
	bool enabled = true;
	bool hovered = false;
	bool selecting = false;
	bool cursor_change = false;

	RichTextbox();
	RichTextbox(int w, int h);
	~RichTextbox() override;
	int GetW() const override;
	int GetH() const override;

	drect vp_show_txt() const;
	dvec p_cursor_rel() const;
	int max_x_cursor_rel() const;
	int max_y_cursor_rel() const;
	int edit_begin() const;
	int edit_end() const;
	wstring left() const;
	wstring mid() const;
	wstring right() const;
	bool selected() const;

	void select_a();
	void select_b();
	void update_x_cursor_rel_hist();

	void hdl_left();
	void hdl_right();
	void hdl_up();
	void hdl_down();
	void hdl_delete();
	void hdl_back();
	void hdl_clip();
	void hdl_copy();
	void hdl_paste();
	void hdl_all();
	void hdl_key();

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
