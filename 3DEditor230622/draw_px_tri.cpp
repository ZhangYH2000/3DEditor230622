#include "draw_px_tri.h"

// 有必要试验一下不同实现方法对于速度的影响。
// 我还不太熟悉参数过于极端时的行为。
// 在测试的时候遇到了无数 bug，可能还有没有发现的 bug。
void sub_draw_px_tri(tile& dest, dbuf& ds, dvec pa, dvec pb, dvec pc,
	double da, double db, double dc, cam const& cm, dcol col, bool same_y) {
	auto ct = cm.ct;
	if (!same_y) { swap(ct.x, ct.y); } // 假设 pa, pb, pc  已经交换过了。
	if (pa.y == pb.y) { return; }
	int sy = pb.y > pa.y ? 1 : -1;
	int sx = pc.x > pb.x ? 1 : -1;
	double y0 = (pa.y - ct.y) * da;
	double y1 = (pb.y - ct.y) * db;
	double y2 = (pc.y - ct.y) * dc;

	for (int y = pa.y; y != pb.y + sy; y += sy) {
		int xb = pa.x * (pb.y - y) + pb.x * (y - pa.y); xb /= (pb.y - pa.y);
		int xc = pa.x * (pb.y - y) + pc.x * (y - pa.y); xc /= (pb.y - pa.y);
		double d0 = interp(y0, y1, y - ct.y, da, db, da, db);
		double d1 = interp(y0, y2, y - ct.y, da, dc, da, dc);
		double x0 = (xb - ct.x) * d0;
		double x1 = (xc - ct.x) * d1;

		for (int x = xb; x != xc + sx; x += sx) {
			int dp = same_y ? 
				y * dest.w + x : x * dest.w + y;
			// 注意这里的约定是 d 为负数，但是这里的参数 da 等都是正数。
			double d = -interp(x0, x1, x - ct.x, d0, d1, d0, d1);
			if (ds[dp] <= d) {  
				ds[dp] = d; dest.cols[dp] = col; 
			}
		}
	}
}
void draw_px_tri(tile& dest, dbuf& ds, dvec pa, dvec pb, dvec pc,
	double da, double db, double dc, cam const& cm, dcol col) {
	// 如果可以让这里写得好看有条理，容易修改且不容易出 bug 就好了。

#define TMP(a, c, x, y, sgn, bd)\
if (p##c.y sgn bd) { return; }\
if (p##a.y sgn bd) {\
	dvec p0 = p##c; p0.y = bd;\
	p0.x += (p##a.x - p##c.x) * (bd - p##c.y) / (p##a.y - p##c.y);\
	double d0 = interp(y##a, y##c, bd - cm.ct.y, d##a, d##c, d##a, d##c);\
	if (pb.y sgn bd) {\
		dvec p1 = p##c; p1.y = bd;\
		p1.x += (pb.x - p##c.x) * (bd - p##c.y) / (p##b.y - p##c.y);\
		double d1 = interp(y##b, y##c, bd - cm.ct.y, d##b, d##c, d##b, d##c);\
		draw_px_tri(dest, ds, p0, p1, p##c, d0, d1, d##c, cm, col); return;\
	}\
	else {\
		dvec p1 = pb; p1.y = bd;\
		p1.x += (p##a.x - pb.x) * (bd - pb.y) / (p##a.y - pb.y);\
		double d1 = interp(y##b, y##a, bd - cm.ct.y, db, d##a, db, d##a);\
		draw_px_tri(dest, ds, p0, pb, p##c, d0, db, d##c, cm, col);\
		draw_px_tri(dest, ds, p0, pb, p1, d0, db, d1, cm, col); return;\
	}\
}

	// 这里有点低效，重复比较了，但是总的影响不大。
	if (pa.x < pb.x) { swap(pa, pb); swap(da, db); }
	if (pa.x < pc.x) { swap(pa, pc); swap(da, dc); }
	if (pb.x < pc.x) { swap(pb, pc); swap(db, dc); }
	if (pa.x == pc.x) { return; }
	int dx = pa.x - pc.x;
	int xa = (pa.x - cm.ct.x) * da;
	int xb = (pb.x - cm.ct.x) * db;
	int xc = (pc.x - cm.ct.x) * dc;
	TMP(a, c, y, x, > , cm.vp.right() - 1);
	TMP(c, a, y, x, < , cm.vp.left());

	if (pa.y < pb.y) { swap(pa, pb); swap(da, db); }
	if (pa.y < pc.y) { swap(pa, pc); swap(da, dc); }
	if (pb.y < pc.y) { swap(pb, pc); swap(db, dc); }
	if (pa.y == pc.y) { return; }
	int dy = pa.y - pc.y;
	int ya = (pa.y - cm.ct.y) * da;
	int yb = (pb.y - cm.ct.y) * db;
	int yc = (pc.y - cm.ct.y) * dc;
	TMP(a, c, x, y, > , cm.vp.bottom() - 1);
	TMP(c, a, x, y, < , cm.vp.top());

#undef TMP
	// 这里是出于数值稳定性的考虑交换一下方向，否则如果三角形很扁的话会奇怪。
	bool y_big = dy > dx; int xya = ya, xyc = yc;
	if (!y_big) { 
		if (pa.x < pb.x) { swap(pa, pb); swap(da, db); }
		if (pa.x < pc.x) { swap(pa, pc); swap(da, dc); }
		if (pb.x < pc.x) { swap(pb, pc); swap(db, dc); }
		xya = (pa.x - cm.ct.x) * da;
		xyc = (pc.x - cm.ct.x) * dc;
		swap(pa.x, pa.y); swap(pb.x, pb.y); swap(pc.x, pc.y); 
	}
	dvec pd = pb;
	pd.x = pa.x * (pb.y - pc.y) + pc.x * (pa.y - pb.y); pd.x /= pa.y - pc.y;
	double cm_xy = y_big ? cm.ct.y : cm.ct.x;
	double dd = interp(xya, xyc, pb.y - cm_xy, da, dc, da, dc);
	sub_draw_px_tri(dest, ds, pa, pb, pd, da, db, dd, cm, col, y_big);
	sub_draw_px_tri(dest, ds, pc, pb, pd, dc, db, dd, cm, col, y_big);
}

void sub_draw_px_tri(tile& dest, dbuf& ds, dvec pa, dvec pb, dvec pc,
	double da, double db, double dc, vec2 va, vec2 vb, vec2 vc,
	cam const& cm, tile const& t, double light, bool same_y) {
	// 这里的纹理坐标是绝对坐标，不是归一化坐标。
	auto ct = cm.ct;
	if (!same_y) { swap(ct.x, ct.y); } // 假设 pa, pb, pc 已经交换过了。
	if (pa.y == pb.y) { return; }
	int sy = pb.y > pa.y ? 1 : -1;
	int sx = pc.x > pb.x ? 1 : -1;
	double y0 = (pa.y - ct.y) * da;
	double y1 = (pb.y - ct.y) * db;
	double y2 = (pc.y - ct.y) * dc;

	for (int y = pa.y; y != pb.y + sy; y += sy) {
		int xb = pa.x * (pb.y - y) + pb.x * (y - pa.y); xb /= (pb.y - pa.y);
		int xc = pa.x * (pb.y - y) + pc.x * (y - pa.y); xc /= (pb.y - pa.y);
		vec2 v0 = interp(y0, y1, y - ct.y, da, db, va, vb);
		vec2 v1 = interp(y0, y2, y - ct.y, da, dc, va, vc);
		double d0 = interp(y0, y1, y - ct.y, da, db, da, db);
		double d1 = interp(y0, y2, y - ct.y, da, dc, da, dc);
		double x0 = (xb - ct.x) * d0;
		double x1 = (xc - ct.x) * d1;

		for (int x = xb; x != xc + sx; x += sx) {
			int dp = same_y ?
				y * dest.w + x : x * dest.w + y;
			// 注意这里的约定是 d 为负数，但是这里的参数 da 等都是正数。
			double d = -interp(x0, x1, x - ct.x, d0, d1, d0, d1);
			if (ds[dp] <= d) {
				ds[dp] = d;
				dvec v = (dvec)interp(x0, x1, x - ct.x, d0, d1, v0, v1);
				v.x = clmp(0, v.x, t.w - 1);
				v.y = clmp(0, v.y, t.h - 1);
				int sp = v.y * t.w + v.x; dcol c = t.cols[sp];
				dest.cols[dp] = dcol(c.r * light, c.g * light, c.b * light);
			}
		}
	}
}
void draw_px_tri(tile& dest, dbuf& ds, dvec pa, dvec pb, dvec pc,
	double da, double db, double dc, vec2 va, vec2 vb, vec2 vc,
	cam const& cm, tile const& t,double light) {

#define TMP(a, c, x, y, sgn, bd)\
if (p##c.y sgn bd) { return; }\
if (p##a.y sgn bd) {\
	dvec p0 = p##c; p0.y = bd;\
	p0.x += (p##a.x - p##c.x) * (bd - p##c.y) / (p##a.y - p##c.y);\
	vec2 v0 = interp(y##a, y##c, bd - cm.ct.y, d##a, d##c, v##a, v##c);\
	double d0 = interp(y##a, y##c, bd - cm.ct.y, d##a, d##c, d##a, d##c);\
	if (pb.y sgn bd) {\
		dvec p1 = p##c; p1.y = bd;\
		p1.x += (pb.x - p##c.x) * (bd - p##c.y) / (p##b.y - p##c.y);\
		vec2 v1 = interp(y##b, y##c, bd - cm.ct.y, d##b, d##c, v##b, v##c);\
		double d1 = interp(y##b, y##c, bd - cm.ct.y, d##b, d##c, d##b, d##c);\
		draw_px_tri(dest, ds, p0, p1, p##c, d0, d1, d##c,\
			v0, v1, v##c, cm, t, light);\
		return;\
	}\
	else {\
		dvec p1 = pb; p1.y = bd;\
		p1.x += (p##a.x - pb.x) * (bd - pb.y) / (p##a.y - pb.y);\
		vec2 v1 = interp(y##b, y##a, bd - cm.ct.y, db, d##a, vb, v##a);\
		double d1 = interp(y##b, y##a, bd - cm.ct.y, db, d##a, db, d##a);\
		draw_px_tri(dest, ds, p0, pb, p##c, d0, db, d##c,\
			v0, vb, v##c, cm, t, light);\
		draw_px_tri(dest, ds, p0, pb, p1, d0, db, d1, v0, vb, v1, cm, t, light);\
		return;\
	}\
}

	// 这里有点低效，重复比较了，但是总的影响不大。
	if (pa.x < pb.x) { swap(pa, pb); swap(da, db); swap(va, vb); }
	if (pa.x < pc.x) { swap(pa, pc); swap(da, dc); swap(va, vc); }
	if (pb.x < pc.x) { swap(pb, pc); swap(db, dc); swap(vb, vc); }
	if (pa.x == pc.x) { return; }
	int dx = pa.x - pc.x;
	int xa = (pa.x - cm.ct.x) * da;
	int xb = (pb.x - cm.ct.x) * db;
	int xc = (pc.x - cm.ct.x) * dc;
	TMP(a, c, y, x, > , cm.vp.right() - 1);
	TMP(c, a, y, x, < , cm.vp.left());

	if (pa.y < pb.y) { swap(pa, pb); swap(da, db); swap(va, vb); }
	if (pa.y < pc.y) { swap(pa, pc); swap(da, dc); swap(va, vc); }
	if (pb.y < pc.y) { swap(pb, pc); swap(db, dc); swap(vb, vc); }
	if (pa.y == pc.y) { return; }
	int dy = pa.y - pc.y;
	int ya = (pa.y - cm.ct.y) * da;
	int yb = (pb.y - cm.ct.y) * db;
	int yc = (pc.y - cm.ct.y) * dc;
	TMP(a, c, x, y, > , cm.vp.bottom() - 1);
	TMP(c, a, x, y, < , cm.vp.top());

#undef TMP
	// 这里是出于数值稳定性的考虑交换一下方向，否则如果三角形很扁的话会奇怪。
	bool y_big = dy > dx; int xya = ya, xyc = yc;
	if (!y_big) {
		if (pa.x < pb.x) { swap(pa, pb); swap(da, db); swap(va, vb); }
		if (pa.x < pc.x) { swap(pa, pc); swap(da, dc); swap(va, vc); }
		if (pb.x < pc.x) { swap(pb, pc); swap(db, dc); swap(vb, vc); }
		xya = (pa.x - cm.ct.x) * da;
		xyc = (pc.x - cm.ct.x) * dc;
		swap(pa.x, pa.y); swap(pb.x, pb.y); swap(pc.x, pc.y);
	}
	dvec pd = pb;
	pd.x = pa.x * (pb.y - pc.y) + pc.x * (pa.y - pb.y); pd.x /= pa.y - pc.y;
	double cm_xy = y_big ? cm.ct.y : cm.ct.x;
	double dd = interp(xya, xyc, pb.y - cm_xy, da, dc, da, dc);
	vec2 vd = interp(xya, xyc, pb.y - cm_xy, da, dc, va, vc);
	sub_draw_px_tri(dest, ds, pa, pb, pd, da, db, dd, va, vb, vd, cm, t, light, y_big);
	sub_draw_px_tri(dest, ds, pc, pb, pd, dc, db, dd, vc, vb, vd, cm, t, light, y_big);
}
