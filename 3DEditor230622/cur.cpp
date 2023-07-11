#include "cur.h"
#include "ui.h"
#include "bgr.h"
#include "geo.h"
#include "par.h"
#include "rhythm.h"
#include "draw_str.h"
#include "draw_3d.h"

Cur::Cur() {
	w = 1800; h = 900;
	set_locale();
	print_console(L"正在加载字体...");
	App::init();
	print_console(L"字体加载完成.", true);
	print_console(L"正在加载控件...");
	// 涉及到传自己的指针，所以创建之后不能复制了。
	mkp(ui)(this); ui->Init();
	mkp(bgr)(this);
	init_fun();
	vol = 0.8;
	show_axis = true;
	light = vec3(0.9, 0, -0.4); amb = 0.2;
	print_console(L"控件加载完成.", true);
	print_console(L"正在加载音乐...");
	mkp(bgm)(load_rhythm_old(L"music/rising.rthm")); 
	print_console(L"音乐加载完成.", true);
}

#define cur this
#include "my_def.h"

void Cur::reset() {
	sc.clear(); init_fun();
	pars.clear(); funs.clear();
	geos.clear(); gsel = NULL;
	tiles.clear(); faces.clear();
}
void Cur::save(wstring const& nm) {
	FILE* f = wfopen(nm, L"wb");
	if (!f) { return; }
	int sz = 0;

	cm.save(f); fwtv(cmd); tmp_cmd = cmd;
	sz = pars.size(); fwt(sz);
	// 可能有些对象在保存时是删除状态，没啥影响。
	for (auto p : pars) { p->save(f); }
	sz = funs.size(); fwt(sz);
	for (auto &fun : funs) { 
		fwtv(fun.first);  fwtv(*fun.second);
	}
	sz = geos.size(); fwt(sz);
	for (auto g : geos) { g->save(f); }

	fclose(f); Execute(sc, Compile(cmd));
}
void Cur::load(wstring const& nm) {
	FILE* f = wfopen(nm, L"rb");
	if (!f) { return; }
	reset(); int sz = 0;

	cm = cam::load(f); 
	frdv(cmd); tmp_cmd = cmd; frd(sz);
	rep(i, 0, sz) { pars.push_back(msh<param>(param::load(f))); }
	frd(sz);
	rep(i, 0, sz) {
		wstring nm; frdv(nm);
		vector<double> vs; frdv(vs);
		funs[nm] = msh<vector<double>>(vs);
	}
	frd(sz);
	rep(i, 0, sz) { geos.push_back(msh<Geo>(Geo::load(this, f))); }
	set_pars_funs(); Execute(sc, Compile(cmd)); fclose(f);
}

ptr<Var> Cur::get_var(wstring const& nm) {
	if (sc.find(nm) == sc.end()) { sc[nm] = msh<Var>(); }
	return sc[nm];
}
ptr<tile> Cur::get_tile(wstring const& nm) {
	if (tiles.find(nm) != tiles.end()) { return tiles[nm]; }
	bool ok = false;
	wstring path = L"images/" + nm + L".tile";
	auto t = tile::load(path, &ok);
	if (!ok) { return {}; }
	tiles[nm] = msh<tile>(t); return tiles[nm];
}
void Cur::set_pars_funs() {
	for (auto& p : pars) { sc[p->nm] = msh<Var>(p->val); }
	for (auto& f : funs) {
		auto pys = f.second;
		auto fun = [pys](vector<ptr<Var>> const& in) {
			if (in.empty()) { return msh<Var>(); }
			auto& ys = *pys;
			auto x = clmp<double>(in[0]->num * (ys.size() - 1), 0.0, ys.size() - 1);
			int x0 = floor(x); int x1 = ceil(x);
			if (x0 == x1) { return msh<Var>(ys[x0]); }
			return msh<Var>(ys[x0] * (x1 - x) + ys[x1] * (x - x0));
		};
		sc[f.first] = msh<Var>(fun);
	}
}

void Cur::set_cam(ptr<Var> c) {
	auto& dic = c->dic;
#define get(nm) if(dic.find(L#nm) != dic.end()) { cm.nm = dic[L#nm]->num; }
	get(theta); get(phi); get(psi); get(scl); get(r);
#undef get
	if (dic.find(L"look") != dic.end()) { cm.look = tv3(*sc[L"look"]); }
}
void Cur::set_light(ptr<Var> l, ptr<Var> a) {
	light = tv3(*l); amb = a->num;
}
void Cur::save_face(ptr<Var> nm, ptr<Var> ts) {
	vector<tri> f;
	for (auto& v : ts->vec) {
		tri t; auto& tri = v->vec;
		if (tri.size() < 3) { continue; }
		t.ua = tv3(*tri[0]);
		t.ub = tv3(*tri[1]);
		t.uc = tv3(*tri[2]);
		if (tri.size() >= 6) {
			t.va = tv2(*tri[3]);
			t.vb = tv2(*tri[4]);
			t.vc = tv2(*tri[5]);
		}
		f.push_back(t);
	}
	faces[nm->str] = msh<vector<tri>>(f);
}
vector<tri> Cur::get_face(wstring const& nm) const {
	return faces.find(nm) == faces.end() ? vector<tri>{} : *faces.at(nm);
}
void Cur::init_fun() {
	auto st_cam = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 1) { set_cam(in[0]); }
		return msh<Var>();
	};
	sc[L"set_cam"] = msh<Var>(st_cam);

	auto st_light = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 2) { set_light(in[0], in[1]); }
		return msh<Var>();
	};
	sc[L"set_light"] = msh<Var>(st_light);

	auto save = [this](vector<ptr<Var>> const& in) {
		if (in.size() >= 2) { save_face(in[0], in[1]); }
		return msh<Var>();
	};
	sc[L"save"] = msh<Var>(save);
}
void Cur::update_curve() {
	auto& curves = *sc[L"curves"];
	for (auto pc : curves.vec) {
		auto &c = pc->dic; 

#define found(s) (c.find(s) != c.end())
		if (!found(L"ps")) { continue; }
		auto& ps = c[L"ps"]->vec;
		
		vector<vec3> vs; col3 col;
		if (found(L"col")) { col = tv3(*c[L"col"]); dbstr = L"df"; }
		for (auto p : ps) { vs.push_back(tv3(*p)); }
		draw_3d_segstrip(scr, dscr, vs, cm, (dcol)col);
	}
#undef found
}

void Cur::render_db() {
	dvec tl_db = bgr.tl + dvec(10, 10);
	int w_db = bgr.w - 20; double d_db = 1000;
	draw_str(scr, dscr, d_db, dbstr, dcol(255), ft, tl_db, w_db, bgr.vp());
}
void Cur::update_db() {
	dbstr = gtvar(L"dbstr")->str;
	render_db();
}
void Cur::Update() {
	bgr.PreUpdate();
	for (auto g : geos) { g->PreUpdate(); }
	ui.PreUpdate();

	if (!paused) { t += dt; }
	sc[L"t"] = msh<Var>(t);
	title = loc(L"title");
	set_pars_funs();
	sc[L"curves"] = msh<Var>();
	auto f = gtvar(L"update"); Execute(sc, f->procs);
#define get(nm) if (sc.find(L#nm) != sc.end()) { nm = sc[L#nm]->num; }
	get(show_axis); get(fixed_light);
#undef get

	pars.erase(remove_if(pars.begin(), pars.end(),
		[](ptr<param> p) { return p->del; }), pars.end());
	geos.erase(remove_if(geos.begin(), geos.end(),
		[](ptr<Geo> g) { return g->del; }), geos.end());

	if (!mute) {
		if (play_rhythm(*this, *bgm, cursp, vol)) { cursp = 0; }
	}

	bgr.Update();
	for (auto g : geos) { g->Update(); }
	update_db();
	update_curve();
	ui.Update();
}
