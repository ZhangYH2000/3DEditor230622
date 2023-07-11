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
// Delegate ���Զ� Discard ���ĳ�Ա���������� CtrlList ��ͬ��һ�㡣
// Delegate Ҫ�� c һֱ����Ч�ģ���ʹ���� c �����ǵ���һ֡��
struct Delegate : Control {
	Control* c = NULL;

	void set_c(Control* _c);
	int GetW() const override;
	int GetH() const override;

	void Update() override;
	void Discard() override;
	void PreUpdate() override;
};
