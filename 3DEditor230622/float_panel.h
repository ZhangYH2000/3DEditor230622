#pragma once
#include "tile.h"
#include "scrollbar.h"

struct App;
struct FloatPanel {
	App* app = NULL;
	dvec tl;
	drect vp;
	double dep = 0;
	Control* c = NULL;
	wstring txt;
	int w = 0, h = 0;
	double wheel_mtp = 0;
	dvec tl_show_rel;
	drect vp_show_raw_rel;

	bool use_sb = false;
	ptr<ScrollbarY> sb;

	int h_bar = 0;
	int s_close = 0;
	int half_s_mark = 0;
	int margin_close = 0;
	int margin_title = 0;
	dcol c_txt, c_mark, c_frame, c_panel;
	dcol c_bar_normal, c_bar_dragged, c_bar_hovered;
	dcol c_close_normal, c_close_chosen, c_close_hovered, c_close_invalid;

	int h_show = 0;
	double t_click = 0;
	bool full = false;
	bool shown = false;
	bool hovered = false;
	bool wheeled = false;
	bool hovered_bar = false;
	bool dragged_bar = false;
	bool chosen_close = false;
	bool hovered_close = false;
	bool enabled_close = true;

	FloatPanel();
	FloatPanel(App* _app, int _w, int _h);
	virtual ~FloatPanel();

	int min_y() const;
	drect vp_show() const;
	dvec tl_close() const;
	void set_c(Control* _c);
	void set_tl(dvec const& p);
	void init_c();
	void init_sb();

	void hide();
	void show();
	void deal_caption();
	void wheel_and_clamp_cy();
	void render_main();
	void render_close();
	void render();

	void Init();
	virtual void Update();
	virtual void PreUpdate();
};
bool less_fpn(FloatPanel* const fp0, FloatPanel* const fp1);
struct FPnLan : FloatPanel {
	wstring id;

	FPnLan() = default;
	FPnLan(App* app, int w, int h, wstring const& id);
	void Update() override;
};
