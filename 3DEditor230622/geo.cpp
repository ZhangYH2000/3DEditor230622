#include "geo.h"
#include "cur.h"
#include "bgr.h"
#include "draw_3d.h"

#include "my_def.h"
#define found(s) (geo.find(s) != geo.end())
#define getval(nm) if (found(L#nm)) { nm = geo[L#nm]->num; }

Geo::Geo(Cur* cur) : cur(cur) {
	p = cm.look;
	tsf = mat3::id();
}
void Geo::save(FILE* f) {
	int sz = 0;
	fwtv(cmd); fwt(p); fwt(tsf);
}
Geo Geo::load(Cur* cur, FILE* f) {
	Geo g(cur); int sz = 0;
	frdv(g.cmd); frd(g.p); frd(g.tsf); 
	g.tmp_cmd = g.cmd; return g;
}
Geo::~Geo() { rmv; }


void Geo::render() {
	vec3 light = cur->fixed_light ? 
		cur->light : cm.mt.tsp() * cur->light;

	for (auto& f : faces) {
		bool bad = f.use_tile && (!f.t || !f.t->n());
		for (auto& t : f.tris) {
			if (bad || hovered || chosen) {
				col3 c =
					chosen ? col3(1, 0, 0) :
					hovered ? col3(0, 0, 1) : col3();
				draw_3d_tri(scr, dscr, t.ua, t.ub, t.uc, cm, c);
			}
			else if (!f.use_tile) {
				draw_3d_tri(scr, dscr, t.ua, t.ub, t.uc, cm, f.c, light, cur->amb);
			}
			else {
				draw_3d_tri(scr, dscr, t.ua, t.ub, t.uc,
					t.va, t.vb, t.vc, cm, *f.t, light, cur->amb);
			}
		}
	}
}
void Geo::transform() {
	for (auto& f : faces)
		for (auto& t : f.tris) {
			t.ua = p + tsf * t.ua;
			t.ub = p + tsf * t.ub;
			t.uc = p + tsf * t.uc;
		}
}

void Geo::refresh_sphere() {
	tri t; auto geo = sc[L"geo"]->dic;
	faces.clear(); faces.resize(1);
	double r = 1;
	int nphi = 40, ntheta = 40;
	getval(r); getval(nphi); getval(ntheta);

	// 顶部和底部会有一些奇异的三角形，但是懒得管了。
	rep(i, 0, nphi) {
		double phi1 = PI * i / nphi;
		double phi0 = PI * (i + 1) / nphi;
		double sp0 = sin(phi0), cp0 = cos(phi0);
		double sp1 = sin(phi1), cp1 = cos(phi1);
		rep(j, 0, ntheta) {
			double theta0 = 2 * PI * j / ntheta;
			double theta1 = 2 * PI * (j + 1) / ntheta;
			double st0 = sin(theta0), ct0 = cos(theta0);
			double st1 = sin(theta1), ct1 = cos(theta1);
			vec3 p0 = r * vec3(ct0 * sp0, st0 * sp0, cp0);
			vec3 p1 = r * vec3(ct1 * sp0, st1 * sp0, cp0);
			vec3 p2 = r * vec3(ct1 * sp1, st1 * sp1, cp1);
			vec3 p3 = r * vec3(ct0 * sp1, st0 * sp1, cp1);

			t.ua = p0; t.ub = p1; t.uc = p2;
			t.va.x = (double)j / ntheta;
			t.va.y = (double)(i + 1) / nphi;
			t.vb.x = (double)(j + 1) / ntheta;
			t.vb.y = (double)(i + 1) / nphi;
			t.vc.x = (double)(j + 1) / ntheta;
			t.vc.y = (double)i / nphi;
			faces[0].tris.push_back(t);
			t.ua = p0; t.ub = p2; t.uc = p3;
			t.va.x = (double)j / ntheta;
			t.va.y = (double)(i + 1) / nphi;
			t.vb.x = (double)(j + 1) / ntheta;
			t.vb.y = (double)i / nphi;
			t.vc.x = (double)j / ntheta;
			t.vc.y = (double)i / nphi;
			faces[0].tris.push_back(t);
		}
	}
}
void Geo::refresh_cylinder() {
	tri t; auto geo = sc[L"geo"]->dic;
	faces.clear(); faces.resize(3);
	double r = 1, l = 1;
	int ntheta = 40;
	getval(r); getval(l); getval(ntheta);

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(-l / 2, -ct0 * r, st0 * r);
		vec3 p1 = vec3(+l / 2, -ct0 * r, st0 * r);
		vec3 p2 = vec3(+l / 2, -ct1 * r, st1 * r);
		vec3 p3 = vec3(-l / 2, -ct1 * r, st1 * r);

		t.ua = p0; t.ub = p1; t.uc = p2;
		t.va.x = 0;
		t.va.y = 1 - (double)i / ntheta;
		t.vb.x = 1;
		t.vb.y = 1 - (double)i / ntheta;
		t.vc.x = 1;
		t.vc.y = 1 - (double)(i + 1) / ntheta;
		faces[0].tris.push_back(t);
		t.ua = p0; t.ub = p2; t.uc = p3;
		t.va.x = 0;
		t.va.y = 1 - (double)i / ntheta;
		t.vb.x = 1;
		t.vb.y = 1 - (double)(i + 1) / ntheta;
		t.vc.x = 0;
		t.vc.y = 1 - (double)(i + 1) / ntheta;
		faces[0].tris.push_back(t);
	}

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(-l / 2, 0, 0);
		vec3 p1 = vec3(-l / 2, -ct0 * r, st0 * r);
		vec3 p2 = vec3(-l / 2, -ct1 * r, st1 * r);

		t.ua = p0; t.ub = p1; t.uc = p2;
		t.va = vec2(0.5);
		t.vb = vec2(0.5) + 0.5 * vec2(ct0, -st0);
		t.vc = vec2(0.5) + 0.5 * vec2(ct1, -st1);
		faces[1].tris.push_back(t);
	}

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(l / 2, 0, 0);
		vec3 p1 = vec3(l / 2, -ct1 * r, st1 * r);
		vec3 p2 = vec3(l / 2, -ct0 * r, st0 * r);

		t.ua = p0; t.ub = p1; t.uc = p2;
		t.va = vec2(0.5);
		t.vb = vec2(0.5) + 0.5 * vec2(-ct1, -st1);
		t.vc = vec2(0.5) + 0.5 * vec2(-ct0, -st0);
		faces[2].tris.push_back(t);
	}
}
void Geo::refresh_box() {
	vec3 p0, p1, p2, p3;
	tri t; auto geo = sc[L"geo"]->dic;
	faces.clear(); faces.resize(6);
	double a = 1, b = 1, c = 1;
	getval(a); getval(b); getval(c);
	
	p0 = vec3(-a, -b, -c) / 2;
	p1 = vec3(+a, -b, -c) / 2;
	p2 = vec3(+a, -b, +c) / 2;
	p3 = vec3(-a, -b, +c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(1, 0);
	faces[0].tris.push_back(t);
	t.ua = p0; t.ub = p2; t.uc = p3;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 0);
	t.vc = vec2(0, 0);
	faces[0].tris.push_back(t);

	p0 = vec3(+a, +b, -c) / 2;
	p1 = vec3(-a, +b, -c) / 2;
	p2 = vec3(-a, +b, +c) / 2;
	p3 = vec3(+a, +b, +c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(1, 0);
	faces[1].tris.push_back(t);
	t.ua = p0; t.ub = p2; t.uc = p3;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 0);
	t.vc = vec2(0, 0);
	faces[1].tris.push_back(t);

	p0 = vec3(-a, +b, -c) / 2;
	p1 = vec3(-a, -b, -c) / 2;
	p2 = vec3(-a, -b, +c) / 2;
	p3 = vec3(-a, +b, +c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(1, 0);
	faces[2].tris.push_back(t);
	t.ua = p0; t.ub = p2; t.uc = p3;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 0);
	t.vc = vec2(0, 0);
	faces[2].tris.push_back(t);

	p0 = vec3(+a, -b, -c) / 2;
	p1 = vec3(+a, +b, -c) / 2;
	p2 = vec3(+a, +b, +c) / 2;
	p3 = vec3(+a, -b, +c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(1, 0);
	faces[3].tris.push_back(t);
	t.ua = p0; t.ub = p2; t.uc = p3;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 0);
	t.vc = vec2(0, 0);
	faces[3].tris.push_back(t);

	p0 = vec3(-a, -b, +c) / 2;
	p1 = vec3(+a, -b, +c) / 2;
	p2 = vec3(+a, +b, +c) / 2;
	p3 = vec3(-a, +b, +c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(1, 0);
	faces[4].tris.push_back(t);
	t.ua = p0; t.ub = p2; t.uc = p3;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 0);
	t.vc = vec2(0, 0);
	faces[4].tris.push_back(t);

	p0 = vec3(-a, +b, -c) / 2;
	p1 = vec3(+a, +b, -c) / 2;
	p2 = vec3(+a, -b, -c) / 2;
	p3 = vec3(-a, -b, -c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(1, 0);
	faces[5].tris.push_back(t);
	t.ua = p0; t.ub = p2; t.uc = p3;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 0);
	t.vc = vec2(0, 0);
	faces[5].tris.push_back(t);
}
void Geo::refresh_cone() {
	tri t; auto geo = sc[L"geo"]->dic;
	faces.clear(); faces.resize(2);
	double r = 1, l = 1;
	int ntheta = 40;
	getval(r); getval(l); getval(ntheta);

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(-l / 2, -ct1 * r, st1 * r);
		vec3 p1 = vec3(-l / 2, -ct0 * r, st0 * r);
		vec3 p2 = vec3(l / 2, 0, 0);

		t.ua = p0; t.ub = p1; t.uc = p2;
		t.va = vec2(0.5) + 0.5 * vec2(ct0, -st0);
		t.vb = vec2(0.5) + 0.5 * vec2(ct1, -st1);
		t.vc = vec2(0.5);
		faces[0].tris.push_back(t);
	}

	rep(i, 0, ntheta) {
		double theta0 = 2 * PI * i / ntheta;
		double theta1 = 2 * PI * (i + 1) / ntheta;
		double st0 = sin(theta0), ct0 = cos(theta0);
		double st1 = sin(theta1), ct1 = cos(theta1);
		vec3 p0 = vec3(-l / 2, 0, 0);
		vec3 p1 = vec3(-l / 2, -ct0 * r, st0 * r);
		vec3 p2 = vec3(-l / 2, -ct1 * r, st1 * r);

		t.ua = p0; t.ub = p1; t.uc = p2;
		t.va = vec2(0.5);
		t.vb = vec2(0.5) + 0.5 * vec2(ct0, -st0);
		t.vc = vec2(0.5) + 0.5 * vec2(ct1, -st1);
		faces[1].tris.push_back(t);
	}
}
void Geo::refresh_pyramid() {
	vec3 p0, p1, p2, p3;
	tri t; auto geo = sc[L"geo"]->dic;
	faces.clear(); faces.resize(5);
	double a = 1, b = 1, c = 1;
	getval(a); getval(b); getval(c);

	p0 = vec3(-a, +b, -c) / 2;
	p1 = vec3(+a, +b, -c) / 2;
	p2 = vec3(+a, -b, -c) / 2;
	p3 = vec3(-a, -b, -c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(1, 0);
	faces[0].tris.push_back(t);
	t.ua = p0; t.ub = p2; t.uc = p3;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 0);
	t.vc = vec2(0, 0);
	faces[0].tris.push_back(t);

	p2 = vec3(0, 0, c) / 2;
	p0 = vec3(-a, -b, -c) / 2;
	p1 = vec3(+a, -b, -c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(0.5, 0);
	faces[1].tris.push_back(t);
	p0 = vec3(+a, +b, -c) / 2;
	p1 = vec3(-a, +b, -c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(0.5, 0);
	faces[2].tris.push_back(t);
	p0 = vec3(-a, +b, -c) / 2;
	p1 = vec3(-a, -b, -c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(0.5, 0);
	faces[3].tris.push_back(t);
	p0 = vec3(+a, -b, -c) / 2;
	p1 = vec3(+a, +b, -c) / 2;
	t.ua = p0; t.ub = p1; t.uc = p2;
	t.va = vec2(0, 1);
	t.vb = vec2(1, 1);
	t.vc = vec2(0.5, 0);
	faces[4].tris.push_back(t);
}
void Geo::refresh_user() {
	auto fs = sc[L"geo"]->vec;
	faces.clear(); faces.resize(fs.size());
	
	rep(i, 0, faces.size()) {
		faces[i].tris = cur->get_face(fs[i]->str);
	}
}
void Geo::refresh() {
	sc[L"geo"] = msh<Var>();
	sc[L"faces"] = msh<Var>();
	Execute(sc, Compile(cmd));
	auto geo = sc[L"geo"]->dic;

	wstring type;
	// sphere, cylinder, box, user, cone, pyramid
	if (found(L"type")) { type = geo[L"type"]->str; }
	if (type == L"sphere") { refresh_sphere(); }
	else if (type == L"cylinder") { refresh_cylinder(); }
	else if (type == L"box") { refresh_box(); }
	else if (type == L"cone") { refresh_cone(); }
	else if (type == L"pyramid") { refresh_pyramid(); }
	else { refresh_user(); }
	transform();

	if (found(L"p")) { p = tv3(*geo[L"p"]); }
	if (found(L"transform")) {
		auto& v = geo[L"transform"]->vec;
		if (v.size() >= 9) {
			tsf.a00 = v[0]->num;
			tsf.a01 = v[1]->num;
			tsf.a02 = v[2]->num;
			tsf.a10 = v[3]->num;
			tsf.a11 = v[4]->num;
			tsf.a12 = v[5]->num;
			tsf.a20 = v[6]->num;
			tsf.a21 = v[7]->num;
			tsf.a22 = v[8]->num;
		}
	}

	auto fs = sc[L"faces"]->vec;
	rep(i, 0, min(faces.size(), fs.size())) {
		auto &v = *fs[i];
		if (v.typ == L"str") {
			faces[i].use_tile = true;
			faces[i].t = gttile(v.str);
		}
		else if (v.typ == L"vec") {
			auto &vec = v.vec;
			if (vec.size() >= 3) {
				faces[i].c.r() = vec[0]->num;
				faces[i].c.g() = vec[1]->num;
				faces[i].c.b() = vec[2]->num;
			}
		}
	}
}

void Geo::PreUpdate() {
	double dep = -DBL_MAX;
	bool hit = false; 
	vec3 p; 
	p.x = msp.x - cm.ct.x;
	p.y = cm.scl * cm.vp.h;
	p.z = -(msp.y - cm.ct.y);
	p = (cm.mt.tsp() * p).unit();

	for (auto& f : faces)
	for (auto& t : f.tris) {
		vec3 v0 = t.ub - t.ua;
		vec3 v1 = t.uc - t.ua;
		vec3 v2 = p;
		vec3 u = mat3(v0, v1, v2).inv() * (cm.p - t.ua);
		bool ok =
			u.x >= 0 && u.y >= 0 && u.x + u.y <= 1 &&
			u.z < -cm.min_d && u.z > dep;
		if (ok) { dep = u.z; hit = true; }
	}

	bool ok = dhv <= dep && hit && insd(msp, bgr.vp()) && show;
	if (ok) { dhv = dep; hvd = this; }
}
void Geo::Update() {
	hovered = hvd == this;
	bool click = msd[0] && !msd_old[0] && hovered;
	if (click) { gsel = this; }
	if (chosen) { chosen = msd[0]; }
	else { chosen = click; }

	if (show) { refresh(); render(); }
}
