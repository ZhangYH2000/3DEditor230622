#pragma once
#include "vec3.h"
#include "tile.h"

struct Cur;
struct Bgr {
	Cur* cur = NULL;
	dvec tl;
	double dep = 0;
	tile black;
	int w = 0, h = 0;

	cam cm;
	bool dragged_mid = false;
	bool dragged_right = false;

	Bgr(Cur* cur);
	drect vp() const { return { tl, w, h }; };
	bool hovered() const;
	bool wheeled() const;

	void render_axis();
	void render();
	void Update();
	void PreUpdate();
};
