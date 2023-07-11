#pragma once
#include "control.h"

struct CtrlListX : Control {
	vector<Control*> cs;

	CtrlListX(vector<Control*> const& cs = {});
	int GetW() const override;
	int GetH() const override;

	void Update() override;
	void Discard() override;
	void PreUpdate() override;
};

struct CtrlListY : Control {
	vector<Control*> cs;

	CtrlListY(vector<Control*> const& cs = {});
	int GetW() const override;
	int GetH() const override;

	void Update() override;
	void Discard() override;
	void PreUpdate() override;
};
