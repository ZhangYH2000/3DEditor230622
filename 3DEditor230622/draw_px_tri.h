#pragma once
#include "vec3.h"
#include "tile.h"

// x0, x1 ָ����ͶӰ֮ǰ�����ꡣxp ��ͶӰ֮������ꡣ
// ���� d0, d1 ̫С������Ƚϴ󣬲�֪���Ƿ�Ҫ����
inline double interp(double x0, double x1, int xp,
	double d0, double d1, double v0, double v1) {
	double t = (xp * d0 - x0) / ((x1 - x0) - xp * (d1 - d0));
	return v0 + t * (v1 - v0);
}
// ���ڸ�������������ܳ��磬��û���ҵ����������Ľ���취��
inline vec2 interp(double x0, double x1, int xp,
	double d0, double d1, vec2 v0, vec2 v1) {
	double t = (xp * d0 - x0) / ((x1 - x0) - xp * (d1 - d0));
	return v0 + t * (v1 - v0);
}

void sub_draw_px_tri(tile& dest, dbuf& ds, dvec pa, dvec pb, dvec pc,
	double da, double db, double dc, cam const& cm, dcol col, bool same_y);
void draw_px_tri(tile& dest, dbuf& ds, dvec pa, dvec pb, dvec pc,
	double da, double db, double dc, cam const& cm, dcol col);

void sub_draw_px_tri(tile& dest, dbuf& ds, dvec pa, dvec pb, dvec pc,
	double da, double db, double dc, vec2 va, vec2 vb, vec2 vc,
	cam const& cm, tile const &t, double light, bool same_y);
void draw_px_tri(tile& dest, dbuf& ds, dvec pa, dvec pb, dvec pc,
	double da, double db, double dc, vec2 va, vec2 vb, vec2 vc,
	cam const& cm, tile const &t, double light);
