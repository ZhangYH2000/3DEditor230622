#include "ctrl_list.h"

CtrlListX::CtrlListX(vector<Control*> const& cs) : cs(cs) {}
int CtrlListX::GetW() const {
	if (cs.empty()) { return 0; }
	int w = 0;
	for (auto c : cs) { w += c->GetW(); }
	return w;
}
int CtrlListX::GetH() const {
	int h = 0;
	for (auto c : cs) {
		if (c->GetH() > h) { h = c->GetH(); }
	}
	return h;
}

void CtrlListX::Update() {
	int cur_x = tl.x;
	for (auto c : cs) {
		c->app = app; c->vp = vp;
		c->tl.x = cur_x; c->tl.y = tl.y;
		c->Update();
		cur_x += c->GetW();
		// 成员是要先 Update 然后再求宽度的。
	}
}
void CtrlListX::Discard() { 
	for (auto c : cs) { c->Discard(); } 
}
void CtrlListX::PreUpdate() {
	for (auto c : cs) {
		c->app = app;
		c->PreUpdate();
	}
}

CtrlListY::CtrlListY(vector<Control*> const& cs) : cs(cs) {}
int CtrlListY::GetW() const {
	int w = 0;
	for (auto c : cs) {
		if (c->GetW() > w) { w = c->GetW(); }
	}
	return w;
}
int CtrlListY::GetH() const {
	if (cs.empty()) { return 0; }
	int h = 0;
	for (auto c : cs) { h += c->GetH(); }
	return h;
}

void CtrlListY::Update() {
	int cur_y = tl.y;
	for (auto c : cs) {
		c->app = app; c->vp = vp;
		c->tl.x = tl.x; c->tl.y = cur_y;
		c->Update();
		cur_y += c->GetH();
	}
}
void CtrlListY::Discard() { 
	for (auto c : cs) { c->Discard(); } 
}
void CtrlListY::PreUpdate() {
	for (auto c : cs) {
		c->app = app;
		c->PreUpdate();
	}
}
