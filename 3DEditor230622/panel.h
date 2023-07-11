#pragma once
#include "tile.h"
#include "scrollbar.h"

struct App;
struct Panel {
	App* app = NULL;
	dvec tl;
	drect vp;
	double dep = 0;
	Control* c = NULL;
	int w = 0, h = 0;
	double wheel_mtp = 0;
	dvec tl_show_rel;
	drect vp_show_raw_rel;
	dcol c_frame, c_panel;

	// ctrl_comp ������ ptr������Ϊ��ͳһҲ�� ptr��
	bool use_sb = false;
	ptr<ScrollbarY> sb;

	int h_show = 0;
	bool full = false;
	bool hovered = false;
	bool wheeled = false;

	Panel();
	virtual ~Panel();
	static Panel menu(App* app);
	static Panel bottom(App* app);

	int min_y() const;
	drect vp_show() const;
	// ���� delegate ��Ҫ��ϣ�������ù�����
	void set_c(Control* _c);
	void init_c();
	void init_sb();
	void render();

	void Init();
	virtual void Update();
	virtual void PreUpdate();
};
