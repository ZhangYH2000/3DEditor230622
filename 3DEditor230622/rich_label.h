#pragma once
#include "font.h"
#include "control.h"

struct RichLabel : Control {
	dvec gap;
	double dep = 0;
	int w = 0, h = 0;
	bool fixed_w = false;
	bool fixed_h = false;

	dcol c_txt;
	wstring txt;

	RichLabel(int w = 0);
	int w_line() const;
	int GetW() const override;
	int GetH() const override;

	void render();
	virtual void Sync() {}
	void Update();
};
struct RLbLan : RichLabel {
	wstring id;

	RLbLan() = default;
	RLbLan(int w, wstring const& id);
	void Sync() override;
};
