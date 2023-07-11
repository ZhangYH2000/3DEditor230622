#pragma once
#include "tile.h"
#include "control.h"

struct App;
// �������ĸ߶ȳ����˷�Χ��Scrollbar �����Զ�������
// ��һȨ������ Panel �ȶ����ֱ���ϼ���
// Scrollbar ���ǿؼ������̳� Control �ࡣ
struct ScrollbarY {
	App* app = NULL;
	dvec tl;
	drect vp;
	double dep = 0;
	Control* c = NULL;
	int w = 0, h = 0;
	int h_show = 0, top_show = 0;
	dcol c_bar, c_frame, c_normal, c_dragged, c_hovered, c_invalid;

	int ny_rel = 0, nh = 0;
	bool full = false;
	bool dragged = false;
	bool enabled = true;
	bool hovered = false;
	bool hovered_node = false;

	ScrollbarY();
	virtual ~ScrollbarY();
	// û�� set_c ���� Discard ֮ǰ�Ķ���
	void render();

	virtual void Update();
	virtual void Discard();
	virtual void PreUpdate();
};
