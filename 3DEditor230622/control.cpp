#include "control.h"

void Delegate::set_c(Control* _c) {
	if (c && c != _c) { c->Discard(); }
	c = _c;
}
int Delegate::GetW() const { return c ? c->GetW() : 0; }
int Delegate::GetH() const { return c ? c->GetH() : 0; }

void Delegate::Update() {
	if (c) {
		// ���������������֣����Ǻ���ʡ�¡�
		// ����Ҳ���Ե�����һ����ʼ��������
		c->app = app; c->tl = tl; c->vp = vp;
		c->Update();
	}
}
void Delegate::Discard() { if (c) { c->Discard(); } }
void Delegate::PreUpdate() {
	if (c) {
		c->app = app;
		c->PreUpdate();
	}
}
