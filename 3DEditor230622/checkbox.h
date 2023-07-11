#pragma once
#include "tile.h"
#include "control.h"

struct Checkbox : Control {
	int h = 0;
	dvec gap;
	double dep = 0;
	int s_box = 0, s_box_in = 0;
	dcol c_frame, c_inner, c_normal, c_invalid, c_hovered;

	bool checked = false;
	bool enabled = true;
	bool hovered = false;

	Checkbox(bool big = false);
	~Checkbox() override;
	int GetW() const override;
	int GetH() const override;

	dvec tl_box() const;
	void render();
	virtual void Sync() {}
	virtual void Upload() const {}

	void Update() override;
	void Discard() override;
	void PreUpdate() override;
};
