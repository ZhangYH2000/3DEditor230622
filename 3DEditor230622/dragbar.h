#pragma once
#include "tile.h"
#include "control.h"

struct App;
struct DragbarX : Control {
	dvec gap;
	double dep = 0;
	int bw = 0, bh = 0;
	int nw = 0, nh = 0;
	dcol c_bar, c_normal, c_hovered, c_invalid, c_dragged;

	int nx_rel = 0;
	bool dragged = false;
	bool enabled = true;
	bool hovered = false;

	DragbarX(int w = 0);
	~DragbarX() override;
	int GetW() const override;
	int GetH() const override;

	double uval() const;
	void set_nx_rel(double uv);
	dvec tl_node() const;
	void render();

	virtual void Sync() {}
	virtual void OnDone() const {}
	virtual void Upload() const {}

	void Update() override;
	void Discard() override;
	void PreUpdate() override;
};
