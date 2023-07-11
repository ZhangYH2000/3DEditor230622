#pragma once
#include "dvec.h"

struct App;
struct Control {
	App* app = NULL;
	dvec tl;
	drect vp;

	virtual void Update() {}
	virtual void Discard() {}
	virtual void PreUpdate() {}

	virtual int GetW() const { return 0; }
	virtual int GetH() const { return 0; }

	virtual ~Control() {}
};
// Delegate 会自动 Discard 它的成员，这是它与 CtrlList 不同的一点。
// Delegate 要求 c 一直是有效的，即使是在 c 被覆盖的那一帧。
struct Delegate : Control {
	Control* c = NULL;

	void set_c(Control* _c);
	int GetW() const override;
	int GetH() const override;

	void Update() override;
	void Discard() override;
	void PreUpdate() override;
};
