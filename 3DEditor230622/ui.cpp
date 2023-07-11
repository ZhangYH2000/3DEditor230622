#include "ui.h"
#include "cur.h"
#include "float_panel.h"
#include "fpn_global.h"
#include "fpn_geo.h"
#include "fpn_par.h"
#include "fpn_fun.h"
#include "fpn_cmd.h"
#include "fpn_language.h"
#include "fpn_about.h"
#include "pn_menu.h"
#include "pn_bottom.h"

#include "my_def.h"

UI::UI(Cur* cur) : cur(cur) {}
void UI::sort_fpns() { sort(fpns.begin(), fpns.end(), less_fpn); }

void UI::Init() {
	mkp(fpn_global)(cur);
	mkp(fpn_geo)(cur);
	mkp(fpn_par)(cur);
	mkp(fpn_fun)(cur);
	mkp(fpn_cmd)(cur);
	mkp(fpn_language)(cur);
	mkp(fpn_about)(cur);

	// pn_menu ���ں�������ΪҪ�� float_panel �ȼ����ꡣ
	// ����Ҫ�� BtFPn ��ԭ��
	// ������ģ�����������������Ҿ���̫�鷳�ˡ�
	mkp(pn_menu)(cur);
	mkp(pn_bottom)(cur);
}
void UI::Update() {
	pn_menu->Update();
	pn_bottom->Update();
	for (auto fp : fpns) { fp->Update(); }
}
void UI::PreUpdate() {
	pn_menu->PreUpdate();
	pn_bottom->PreUpdate();
	sort_fpns();
	for (auto fp : fpns) { fp->PreUpdate(); }
}
