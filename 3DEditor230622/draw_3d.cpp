#include "draw_3d.h"
#include "draw_px_seg.h"
#include "draw_px_tri.h"

void sub_draw_3d_tri(tile& dest, dbuf& ds,
	vec3 pa, vec3 pb, vec3 pc, cam const& cm, dcol col) {

#define TMP(a, c, sgn, max_d)\
	if (p##c.y sgn cm.max_d) { return; }\
	if (p##a.y sgn cm.max_d) {\
		vec3 p0 = p##c; p0.y = cm.max_d;\
		p0.x += (p##a.x - p##c.x) * (cm.max_d - p##c.y) / (p##a.y - p##c.y);\
		p0.z += (p##a.z - p##c.z) * (cm.max_d - p##c.y) / (p##a.y - p##c.y);\
		if (pb.y sgn cm.max_d) {\
			vec3 p1 = p##c; p1.y = cm.max_d;\
			p1.x += (pb .x - p##c.x) * (cm.max_d - p##c.y) / (pb.y - p##c.y);\
			p1.z += (pb .z - p##c.z) * (cm.max_d - p##c.y) / (pb.y - p##c.y);\
			sub_draw_3d_tri(dest, ds, p0, p1, p##c, cm, col);\
		}\
		else {\
			vec3 p1 = pb; p1.y = cm.max_d;\
			p1.x += (p##a.x - pb.x) * (cm.max_d - pb.y) / (p##a.y - pb.y);\
			p1.z += (p##a.z - pb.z) * (cm.max_d - pb.y) / (p##a.y - pb.y);\
			sub_draw_3d_tri(dest, ds, p0, pb, p##c, cm, col);\
			sub_draw_3d_tri(dest, ds, p0, pb, p1, cm, col);\
		}\
	}

	// 这里的严格不等号很重要，否则会死循环。
	// 这里有点低效，重复比较了，但是总的影响不大。
	TMP(a, c, > , max_d); 
	TMP(c, a, < , min_d);

#undef TMP
	dvec p0 = proj(pa, cm);
	dvec p1 = proj(pb, cm);
	dvec p2 = proj(pc, cm);
	draw_px_tri(dest, ds, p0, p1, p2, pa.y, pb.y, pc.y, cm, col);
}
void draw_3d_tri(tile& dest, dbuf& ds, vec3 pa, vec3 pb, vec3 pc,
	cam const& cm, col3 col, vec3 v_light, double env_light) {
	auto n = cross((pb - pa), (pc - pa));
	if (dot(cm.p - pa, n) <= 0) { return; }

	pa = cm.mt * (pa - cm.p);
	pb = cm.mt * (pb - cm.p);
	pc = cm.mt * (pc - cm.p);
	double light = max(0.0, dot(n.unit(), -v_light));
	light = min(1.0, light + env_light);
	dcol c = dcol(col * light);

	if (pa.y < pb.y) { swap(pa, pb); }
	if (pa.y < pc.y) { swap(pa, pc); }
	if (pb.y < pc.y) { swap(pb, pc); }
	sub_draw_3d_tri(dest, ds, pa, pb, pc, cm, c);
}

void sub_draw_3d_tri(tile& dest, dbuf& ds,
	vec3 pa, vec3 pb, vec3 pc, vec2 va, vec2 vb, vec2 vc,
	cam const& cm, tile const& t, double light) {

#define TMP(a, c, sgn, max_d)\
	if (p##c.y sgn cm.max_d) { return; }\
	if (p##a.y sgn cm.max_d) {\
		vec3 p0 = p##c; p0.y = cm.max_d;\
		p0.x += (p##a.x - p##c.x) * (cm.max_d - p##c.y) / (p##a.y - p##c.y);\
		p0.z += (p##a.z - p##c.z) * (cm.max_d - p##c.y) / (p##a.y - p##c.y);\
		vec2 v0 = v##c + (v##a - v##c) * (cm.max_d - p##c.y) / (p##a.y - p##c.y);\
		if (pb.y sgn cm.max_d) {\
			vec3 p1 = p##c; p1.y = cm.max_d;\
			p1.x += (pb .x - p##c.x) * (cm.max_d - p##c.y) / (pb.y - p##c.y);\
			p1.z += (pb .z - p##c.z) * (cm.max_d - p##c.y) / (pb.y - p##c.y);\
			vec2 v1 = v##c + (v##b - v##c) * (cm.max_d - p##c.y) / (p##b.y - p##c.y);\
			sub_draw_3d_tri(dest, ds, p0, p1, p##c, v0, v1, v##c, cm, t, light);\
		}\
		else {\
			vec3 p1 = pb; p1.y = cm.max_d;\
			p1.x += (p##a.x - pb.x) * (cm.max_d - pb.y) / (p##a.y - pb.y);\
			p1.z += (p##a.z - pb.z) * (cm.max_d - pb.y) / (p##a.y - pb.y);\
			vec2 v1 = v##b + (v##a - v##b) * (cm.max_d - p##b.y) / (p##a.y - p##b.y);\
			sub_draw_3d_tri(dest, ds, p0, pb, p##c, v0, vb, v##c, cm, t, light);\
			sub_draw_3d_tri(dest, ds, p0, pb, p1, v0, vb, v1, cm, t, light);\
		}\
	}

	// 这里的严格不等号很重要，否则会死循环。
	// 这里有点低效，重复比较了，但是总的影响不大。
	TMP(a, c, > , max_d);
	TMP(c, a, < , min_d);

#undef TMP
	dvec p0 = proj(pa, cm);
	dvec p1 = proj(pb, cm);
	dvec p2 = proj(pc, cm);
	draw_px_tri(dest, ds, p0, p1, p2, pa.y, pb.y, pc.y, va, vb, vc, cm, t, light);
}
void draw_3d_tri(tile& dest, dbuf& ds,
	vec3 pa, vec3 pb, vec3 pc, vec2 va, vec2 vb, vec2 vc,
	cam const& cm, tile const& t, vec3 v_light, double env_light) {
	auto n = cross((pb - pa), (pc - pa));
	if (dot(cm.p - pa, n) <= 0) { return; }

	pa = cm.mt * (pa - cm.p);
	pb = cm.mt * (pb - cm.p);
	pc = cm.mt * (pc - cm.p);
	double light = max(0.0, dot(n.unit(), -v_light));
	light = min(1.0, light + env_light);

	if (pa.y < pb.y) { swap(pa, pb); swap(va, vb); }
	if (pa.y < pc.y) { swap(pa, pc); swap(va, vc); }
	if (pb.y < pc.y) { swap(pb, pc); swap(vb, vc); }
	vec2 wh = vec2(t.w, t.h); va *= wh; vb *= wh; vc *= wh;
	sub_draw_3d_tri(dest, ds, pa, pb, pc, va, vb, vc, cm, t, light);
}

void draw_3d_seg(tile& dest, dbuf& ds, vec3 pa, vec3 pb, cam const& cm, dcol c) {
	pa = cm.mt * (pa - cm.p);
	pb = cm.mt * (pb - cm.p);
	if (pa.y > pb.y) { swap(pa, pb); }

#define TMP(a, b, sgn, min_d)\
	if (p##b.y sgn cm.min_d) { return; }\
	if (p##a.y sgn cm.min_d) {\
		p##a.x += (p##b.x - p##a.x) * (cm.min_d - p##a.y) / (p##b.y - p##a.y);\
		p##a.z += (p##b.z - p##a.z) * (cm.min_d - p##a.y) / (p##b.y - p##a.y);\
		p##a.y = cm.min_d;\
	}

	TMP(a, b, < , min_d);
	TMP(b, a, > , max_d);
	dvec p0 = proj(pa, cm);
	dvec p1 = proj(pb, cm);
	draw_px_seg(dest, ds, p0, p1, pa.y, pb.y, cm, c);
}
void draw_3d_segstrip
(tile& dest, dbuf& ds, vector<vec3> const& ps, cam const& cm, dcol c) {
	vec3 p0 = ps.front();
	for (auto p1 : ps) {
		draw_3d_seg(dest, ds, p0, p1, cm, c); p0 = p1;
	}
}
