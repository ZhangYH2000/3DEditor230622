#include "vec3.h"

vec3 vec3::from_str(wstring const& s) {
	vec3 v; int i = 0;
	v.x = fetch_num(i, s);
	v.y = fetch_num(i, s);
	v.z = fetch_num(i, s);
	return v;
}
wstring tw2(vec3 v) {
	return L"(" + tw2(v.x) + L"," + tw2(v.y) + L"," + tw2(v.z) + L")";
}

cam::cam(drect vp) : vp(vp) {
	r = 1000; scl = 500;
	max_d = 1e6; min_d = 1; eps = 1e-3;
	calc();
}
void cam::calc() {
	p.y = -cos(theta) * cos(phi);
	p.x = sin(theta) * cos(phi);
	p.z = sin(phi);
	p = p * r + look;

	// 下面可能会产生奇异的情况。
	vec3 vy = (look - p).unit();
	vec3 vx = cross(vy, vec3(0, 0, 1)).unit();
	vec3 vz = cross(vx, vy);
	mt = mat3(vx, vy, vz).tsp();
	mt = mat3::rot(vec3(0, 1, 0), psi) * mt;
	ct = vp.tl + dvec(vp.w, vp.h) / 2;
}
void cam::save(FILE* f) {
	fwt(look); fwt(vp); fwt(psi); fwt(scl);
	fwt(theta); fwt(phi); fwt(r);
	fwt(min_d); fwt(max_d); fwt(eps);
}
cam cam::load(FILE* f) {
	cam cm;
	frd(cm.look); frd(cm.vp); frd(cm.psi); frd(cm.scl);
	frd(cm.theta); frd(cm.phi); frd(cm.r);
	frd(cm.min_d); frd(cm.max_d); frd(cm.eps);
	cm.calc(); return cm;
}
