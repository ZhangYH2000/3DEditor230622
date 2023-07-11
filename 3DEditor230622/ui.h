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

	// 这里用 init 的原因是在构造函数中传自己的指针可能会出问题。
	// 因为控件会用 cur->ui 来访问 ui, 但这个时候 ui 尚未初始化。
	// 这也是为什么主程序不需要 Init。
	void Init();
	void Update();
	void PreUpdate();
};
