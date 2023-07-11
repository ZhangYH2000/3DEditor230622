#pragma once
#include "app.h"
#include "var.h"

struct UI;
struct Bgr;
struct Geo;
struct tri;
struct param;
struct rhythm;
struct Cur : App {
	ptr<UI> ui;
	ptr<Bgr> bgr;
	Scope sc;
	wstring dbstr, cmd, tmp_cmd;
	
	double t = 0;
	bool paused = false;
	bool show_axis = false;
	bool fixed_light = false;
	vector<ptr<param>> pars;
	map<wstring, ptr<vector<double>>> funs;
	
	double amb = 0;
	vec3 light;

	bool mute = false;
	int cursp = 0;
	double vol = 0;
	ptr<rhythm> bgm;
	
	Geo* geo_selected = NULL;
	vector<ptr<Geo>> geos;
	map<wstring, ptr<tile>> tiles;
	map<wstring, ptr<vector<tri>>> faces;

	Cur();
	void reset();
	void save(wstring const &nm);
	void load(wstring const &nm);
	ptr<Var> get_var(wstring const& nm);
	ptr<tile> get_tile(wstring const& nm);
	void set_pars_funs();

	void set_cam(ptr<Var> c);
	void set_light(ptr<Var> l, ptr<Var> a);
	void save_face(ptr<Var> nm, ptr<Var> ts);
	vector<tri> get_face(wstring const& nm) const;
	void init_fun();
	void update_curve();

	void render_db();
	void update_db();
	void Update() override;
};
