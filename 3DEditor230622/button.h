#pragma once
#include "font.h"
#include "control.h"

struct Button : Control {
	dvec gap;
	double dep = 0;
	wstring txt;
	int w = 0, h = 0;
	dcol c_txt, c_frame, c_normal, c_chosen, c_hovered, c_invalid;

	bool chosen = false;
	bool enabled = true;
	bool hovered = false;

	Button(int w = 0);
	~Button() override;
	int GetW() const override;
	int GetH() const override;

	void render();
	virtual void OnClick() {}

	void Update() override;
	void Discard() override;
	void PreUpdate() override;
};
struct BtLan : Button {
	wstring id;

	BtLan() = default;
	BtLan(int w, wstring const& id);
	void Update() override;
};

struct FloatPanel;
struct BtFPn : Button {
	FloatPanel* fpn = NULL;

	BtFPn() = default;
	BtFPn(FloatPanel* fpn);
	void OnClick() override;
	void Update() override;
};
