#pragma once
#include "tile.h"

struct tri {
	vec3 ua, ub, uc;
	vec2 va, vb, vc;
};
struct face {
	col3 c;
	ptr<tile> t;
	bool use_tile = false;
	vector<tri> tris;
};

struct Cur;
struct Geo {
	Cur* cur = NULL;
	vec3 p;
	mat3 tsf;
	vector<face> faces;

	wstring cmd, tmp_cmd;
	bool del = false;
	bool show = true;
	bool chosen = false;
	bool hovered = false;
	
	Geo(Cur* cur);
	void save(FILE* f);
	static Geo load(Cur* cur, FILE* f);
	virtual ~Geo();
	
	void render();
	void transform();
	void refresh_sphere();
	void refresh_cylinder();
	void refresh_box();
	void refresh_cone();
	void refresh_pyramid();
	void refresh_user();
	void refresh();

	void PreUpdate();
	void Update();
};
