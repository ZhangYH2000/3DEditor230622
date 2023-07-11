#pragma once
#include "utils.h"

struct Cur;
struct FloatPanel;
struct FPnGlobal;
struct FPnGeo;
struct FPnPar;
struct FPnFun;
struct FPnCmd;
struct FPnLanguage;
struct FPnAbout;
struct PnMenu;
struct PnBottom;
struct UI {
	Cur* cur = NULL;
	vector<FloatPanel*> fpns;

	ptr<FPnGlobal> fpn_global;
	ptr<FPnGeo> fpn_geo;
	ptr<FPnPar> fpn_par;
	ptr<FPnFun> fpn_fun;
	ptr<FPnCmd> fpn_cmd;
	ptr<FPnLanguage> fpn_language;
	ptr<FPnAbout> fpn_about;
	ptr<PnMenu> pn_menu;
	ptr<PnBottom> pn_bottom;

	UI(Cur* cur);
	void sort_fpns();

	// ������ init ��ԭ�����ڹ��캯���д��Լ���ָ����ܻ�����⡣
	// ��Ϊ�ؼ����� cur->ui ������ ui, �����ʱ�� ui ��δ��ʼ����
	// ��Ҳ��Ϊʲô��������Ҫ Init��
	void Init();
	void Update();
	void PreUpdate();
};
