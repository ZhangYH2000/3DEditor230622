#pragma once
#include "panel.h"
#include "label.h"
#include "ctrl_list.h"

struct LbFPS : Label {
	LbFPS();
	void Sync() override;
};
struct LbCam : Label {
	LbCam();
	void Sync() override;
};

struct PnBottom : Panel {
	ptr<LbFPS> lb_fps;
	ptr<LbCam> lb_cam;
	ptr<CtrlListX> clx;

	PnBottom(App* app);
};
