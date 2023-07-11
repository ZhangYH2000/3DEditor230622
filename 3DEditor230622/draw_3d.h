#pragma once
#include "vec3.h"
#include "tile.h"

inline dvec proj(vec3 p, cam const& cm) {
	// 这里的 p 是与 mt 相乘之后的。
	vec2 tmp = vec2(p.x, -p.z) / p.y * cm.scl * cm.vp.h;
	return cm.ct + dvec(tmp);
}

void sub_draw_3d_tri(tile& dest, dbuf& ds, 
	vec3 pa, vec3 pb, vec3 pc, cam const& cm, dcol col);
void draw_3d_tri(tile& dest, dbuf& ds, vec3 pa, vec3 pb, vec3 pc, 
	cam const& cm, col3 col, vec3 v_light = {}, double env_light = 1);

void sub_draw_3d_tri(tile& dest, dbuf& ds,
	vec3 pa, vec3 pb, vec3 pc, vec2 va, vec2 vb, vec2 vc,
	cam const& cm, tile const& t, double light);
void draw_3d_tri(tile& dest, dbuf& ds,
	vec3 pa, vec3 pb, vec3 pc, vec2 va, vec2 vb, vec2 vc, 
	cam const& cm, tile const& t, vec3 v_light = {}, double env_light = 1);

void draw_3d_seg(tile& dest, dbuf& ds, vec3 pa, vec3 pb, cam const& cm, dcol c);
void draw_3d_segstrip
(tile& dest, dbuf& ds, vector<vec3> const& ps, cam const& cm, dcol c);
