#pragma once
#include "font.h"
#include "control.h"

struct Label : Control {
	dvec gap;
	double dep = 0;
	int w = 0, h = 0;
	bool fixed_w = false;

	dcol c_txt;
	wstring txt;

	Label(bool big = false);
	int GetW() const override;
	int GetH() const override;

	void render();
	virtual void Sync() {}
	void Update() override;
};
struct LbLan : Label {
	wstring id;

	LbLan() = default;
	LbLan(wstring const& id, bool big = false);
	void Sync() override;
};
